#ifdef DEBUG
#pragma once

#include <string_view>
#include <iostream>

namespace spell_checker::debug_utils
{
   using std::cout;
   using std::endl;
   using std::string_view;

   // requires
   template <typename Cont>
   void printCont(string_view name, const Cont& cont)
   {
      struct printer: Cont
      {
         void print(string_view name) const
         {
            cout << "name: " << name << endl;
            for (const auto& a : *this)
               cout << a << ',';
            cout << endl;
         }
      };

      static_cast<const printer&>(cont).print(name);
   }
} // namespace spell_checker::debug_utils

#endif
