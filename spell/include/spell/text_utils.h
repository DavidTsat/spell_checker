#pragma once

#include <string>
#include <string_view>
#include <variant>
#include <cctype>
#include <iterator>

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

      size_t operator()(string_view s) const // simple hash to fulfill the equality
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

   template <typename InIt>
   size_t getSepLen(InIt first, InIt last)
   {
      if (first == last)
         return 0;

      if (isspace(static_cast<unsigned char>(*first)))
         return 1;

      if (*first == '\\')
      {
         auto nextIt = next(first);
         if (nextIt != last)
         {
            char c = *nextIt;
            if (c == 'r' || c == 'n' || c == 't' || c == 'f' || c == 'v') // the same characters checked by isspace
               return 2;
         }
      }
      return 0;
   }

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

         string res{string(*begin)};
         for (auto it = next(begin); it != end; ++it)
         {
            res += sep;
            res += string(*it);
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

   template <bool StorePrefix = false, typename InIt, typename OutIt>
   InIt tokenize(InIt first, InIt last, OutIt dest, string_view term, bool skipAllLeadingSeps = false)
   {
      while (first != last)
      {
         if (skipAllLeadingSeps)
         {
            while (first != last)
            {
               size_t len = getSepLen(first, last);
               if (len == 0)
                  break;
               advance(first, len);
            }
            if (first == last)
               break;
         }

         InIt tokenStart = first;

         if (!skipAllLeadingSeps)
         {
            while (first != last)
            {
               size_t len = getSepLen(first, last);
               if (len == 0)
                  break;
               advance(first, len);
            }
         }

         InIt wordStart = first;
         InIt wordEnd = first;
         while (wordEnd != last && getSepLen(wordEnd, last) == 0)
            ++wordEnd;

         if (tokenStart == wordEnd)
            break;
         
         string_view prefix{&*tokenStart, static_cast<size_t>(distance(tokenStart, wordStart))};
         string_view currWord{&*wordStart, static_cast<size_t>(distance(wordStart, wordEnd))};

         if (size_t e = currWord.find(term); e != string_view::npos)
         {
            if (e > 0)
            {
               string_view trimmedWord = currWord.substr(0, e);
               if constexpr (StorePrefix)
                  *dest = make_pair(trimmedWord, prefix);
               else
                  *dest = trimmedWord;
            }
            first = wordStart + e + term.size();
            break;
         }

         if constexpr (StorePrefix)
            *dest = make_pair(currWord, prefix);
         else
            *dest = currWord;

         first = wordEnd;
      }

      return first;
   }
} // namespace spell::text
