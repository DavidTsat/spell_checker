#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <cctype>

namespace spell::text
{
   using namespace std;

   struct CaseInsensitiveMatch
   {
      bool operator()(char a, char b) const
      {
         return static_cast<unsigned char>(tolower(a)) == static_cast<unsigned char>(tolower(b));
      }

      bool operator()(string_view a, string_view b) const
      {
         if (a.size() != b.size())
            return false;

         return equal(a.begin(), a.end(), b.begin(), *this);
      }

      size_t operator()(string_view s) const
      {
         size_t h = 0;
         for (char c : s)
            h = h * 31 + static_cast<unsigned char>(tolower(c));

         return h;
      }
   };

   struct CaseSensitiveMatch
   {
      bool operator()(char a, char b) const
      {
         return static_cast<unsigned char>(a) == static_cast<unsigned char>(b);
      }

      bool operator()(string_view a, string_view b) const
      {
         return a == b;
      }

      size_t operator()(string_view s) const
      {
         return hash<string_view>{}(s);
      }
   };

   using CharMatchPolicy = variant<struct CaseInsensitiveMatch, struct CaseSensitiveMatch>;

   // pre and post processor: left trim whitespaces while preserving the original prefix
   struct LTrimWhitespace
   {
      struct Result
      {
         string trimmed;
         string prefix;
      };

      Result preprocess(string_view word) const
      {
         size_t i = 0;
         while (i != word.size() && isspace(static_cast<unsigned char>(word[i])))
            ++i;

         return {string(word.substr(i)), string(word.substr(0, i))};
      }

      string postprocess(string_view prefix, string_view word) const
      {
         // if (!word.empty() && word[0] == '{')
         // {
         //    if (prefix.empty())
         //       return string(word);

         //    return string(1, prefix[0]) + "{" + string(prefix.substr(1)) + string(word.substr(1));
         // }

         return string(prefix) + string(word);
      }
   };

   template <typename InIt>
   string format(InIt beginD1, InIt endD1, InIt beginD2, InIt endD2, char sep)
   {
      auto join = [](InIt begin, InIt end, string sep) -> auto
      {
         if (begin == end)
            return string{};

         string res{*begin};
         for (auto it = next(begin); it != end; ++it)
         {
            res += sep;
            res += *it;
         }

         if (distance(begin, end) > 1)
            return '{' + res + '}';
         return res;
      };

      string res{join(beginD1, endD1, string(1, sep))};
      if (!res.empty())
         return res;

      return join(beginD2, endD2, string(1, sep));
   }

   template <typename InIt, typename OutIt>
   InIt tokenize(InIt first, InIt last, OutIt dest, string_view term, bool skipAllLeadingSeps = false)
   {
      while (first != last)
      {
         if (skipAllLeadingSeps)
         {
            while (first != last && isspace(static_cast<unsigned char>((*first))))
               ++first;

            if (first == last)
               break;
         }
         else if (isspace(static_cast<unsigned char>((*first))))
            ++first;
         InIt f2 = first;
         while (f2 != last && isspace(static_cast<unsigned char>(*f2)))
            ++f2;
         while (f2 != last && !isspace(static_cast<unsigned char>(*f2)))
            ++f2;

         string_view currWord{first, f2};

         if (size_t e = currWord.find(term); e != string_view::npos)
         {
            if (!(e == 0))
               *dest = string_view{currWord.data(), e};
            first = first + e + term.size();
            break;
         }

         *dest = currWord;

         first = f2;
      }

      return first;
   }
} // namespace spell::text