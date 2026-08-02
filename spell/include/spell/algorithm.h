#pragma once

#include <string_view>
#include <cstdint>

namespace spell_checker::algorithm
{
   using std::string_view;

   constexpr size_t MAX_LEN = 50;
   // having max 50 lenght, the maximum weight can be 100 - all elements to replace
   inline constexpr uint8_t INF = 250; // suitable for uint8_t: inf + cost < 255

   // to test transitivity as well
   int dist(string_view a, string_view b);
} // namespace spell_checker::algorithm
