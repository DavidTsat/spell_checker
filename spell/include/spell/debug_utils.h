#ifdef DEBUG
#pragma once

#include <string_view>
#include <iostream>

namespace spell::debug_utils
{
   using std::cout;
   using std::endl;
   using std::string_view;

   // requires
   template <typename Cont>
   void printCont(string_view name, const Cont& cont)
   {
      for (const auto& a : cont)
         cout << a << ',';
      cout << endl;
   }
} // namespace spell::debug_utils

#endif