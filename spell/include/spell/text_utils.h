#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <variant>

namespace spell_checker::text
{
   using std::string;
   using std::string_view;
   using std::vector;

   // as this is called for every character, we better not to use type-erased polymorphic std::function or  an interface with a virtual comparator function
   struct CaseInsensitiveMatch
   {
      inline bool operator()(char a, char b) const
      {
         return static_cast<unsigned char>(std::tolower(a))
             == static_cast<unsigned char>(std::tolower(b)); // todo to match also non-printable binary data
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

   // can be generic
   string format(const vector<string_view>& d1, const vector<string_view>& d2);

   using InIt = string_view::const_iterator;
   template <typename OutIt>
   InIt tokenize(InIt first, InIt last, OutIt dest, char sep, string_view term)
   {
      while (first != last)
      {
         // skip over separators
         while (first != last && *first == sep)
            ++first;

         InIt f2 = first;
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
} // namespace spell_checker::text
