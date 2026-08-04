#ifdef DEBUG
#pragma once

#include <string_view>
#include <iostream>

namespace spell::debug_utils
{
   using namespace std;

   // requires
   template <typename Cont>
   void printCont(string_view name, const Cont& cont)
   {
      for (const auto& a : cont)
         cout << a << ',';
      cout << endl;
   }

   template <size_t MAX_LEN, typename Cont>
   void backtrackAndPrint(uint8_t (&bt)[MAX_LEN + 1][MAX_LEN + 1][3], const Cont& ops)
   {
      // todo: to parse the bt and print.
   }
} // namespace spell::debug_utils

#endif
