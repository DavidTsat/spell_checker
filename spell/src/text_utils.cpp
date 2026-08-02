#include "spell/text_utils.h"

namespace spell_checker::text
{
   string format(const vector<string_view>& d1, const vector<string_view>& d2)
   {
      auto join = [](const vector<string_view>& vec, char sep) -> string
      {
         if (vec.empty())
            return {};

         string res{vec.front()};
         for (auto it = next(vec.cbegin()); it != vec.cend(); ++it)
         {
            res += sep;
            res += *it;
         }

         if (vec.size() > 1)
            return '{' + res + '}';
         return res;
      };

      string res{join(d1, ' ')};
      if (!res.empty())
         return res;

      return join(d2, ' ');
   }
} // namespace spell_checker::text
