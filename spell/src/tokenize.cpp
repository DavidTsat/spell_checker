#include "spell/tokenize.h"

namespace spell_check::text
{
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
} // namespace spell_check::text
