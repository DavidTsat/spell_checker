#pragma once
#include "spell/text_utils.h"

#include <string_view>
#include <cstdint>

namespace spell::algorithm
{
   using namespace spell::text;

   using std::string_view;

   inline constexpr size_t MAX_LEN = 50;

   // having max 50 lenght, the maximum weight can be 100 - all elements to replace
   inline constexpr uint8_t INF = 250; // suitable for uint8_t: inf + cost < 255

   /*
      * Computes the edit distance (the weight of operations: insertion 1, deletion 1, substitution 2) between two words.
      * Allowed operations: insertion, deletion, substitution (subtitution is counted as 2 operations: deletion + insertion).
      * Two consecutive insertions or deletions are not allowed.
      * Returns -1 if the distance is greater than 2.
      * Throws std::out_of_range if the length of the words is greater than MAX_LEN (50).

      * The algorithm is based on Lavenshtein distance, but with the additional constraint that two consecutive insertions or deletions are not allowed.
      * To fullfill this constraing, the algorithm uses a 3D dynamic programming matrix, where the third dimension represents the last operation performed (match/substitution, insertion, deletion).
      * The 3-rd dimension being a small state machine, doesn't allow transitions from insertion to insertion or deletion to deletion, thus preventing two consecutive insertions or deletions.
      * The time complexity is O(n*m) (strinctly speaking O(n*m*3)), where n and m are the lengths of the. A drawback is that the algorithm doesn't have an early stop,
      * that is, even if the distance is already greater than 2, it will continue to compute the entire matrix and return -1 at the end. This can be optimized.
   */
   int dist(string_view a, string_view b, CharMatchPolicy charMatchPolicy = CaseInsensitiveMatch{});
} // namespace spell::algorithm
