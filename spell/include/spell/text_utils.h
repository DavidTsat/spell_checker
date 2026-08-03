#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <variant>

namespace spell::text
{
   using std::string;
   using std::string_view;
   using std::vector;
   using std::tolower;
   using std::next;
   using std::distance;
   
   // as this is called for every character, we better not to use type-erased polymorphic std::function or  an interface with a virtual comparator function
   struct CaseInsensitiveMatch
   {
      inline bool operator()(char a, char b) const
      {
         return static_cast<unsigned char>(tolower(a))
             == static_cast<unsigned char>(tolower(b)); // todo to match also non-printable binary data either here, or introduce a new policy
      }
   };

   // as this is called for every character, we better not to use type-erased polymorphic std::function or  an interface with a virtual comparator function
   struct CaseSensitiveMatch
   {
      inline bool operator()(char a, char b) const
      {
         return static_cast<unsigned char>(a) == static_cast<unsigned char>(b);
      }
   };

   using CharMatchPolicy = std::variant<struct CaseInsensitiveMatch, struct CaseSensitiveMatch>;

   // pre and post processor: left trim whitespaces
   struct LTrimWhitespace
   {
      struct Result
      {
         string_view trimmed;
         string_view prefix;
      };

      Result preprocess(string_view word) const
      {
         static constexpr const char* whitespace = " \t\n\r\f\v";

         auto begin = word.find_first_not_of(whitespace);
         if (begin == string_view::npos)
            return {};

         return {word.substr(begin), word.substr(0, begin)};
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
   InIt tokenize(InIt first, InIt last, OutIt dest, char sep, string_view term)
   {
      while (first != last)
      {
         // skip over one separator
         if (first != last && *first == sep)
            ++first;

         InIt f2 = first;
         while (f2 != last && *f2 == sep)
            ++f2;
         while (f2 != last && *f2 != sep)
            ++f2;

         string_view currWord{first, f2};

         if (std::size_t e = currWord.find(term); e != string_view::npos)
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
