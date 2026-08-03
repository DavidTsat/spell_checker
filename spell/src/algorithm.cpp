#include "spell/algorithm.h"

#include <algorithm>
#include <cstring>

namespace spell::algorithm
{
   // anonymous namespace to hide the implementation details
   namespace
   {
      using std::memset;
      using std::min;
      using std::out_of_range;

      template <typename CharMatchPolicy>
      int distImpl(string_view a, string_view b, const CharMatchPolicy& matchPolicy)
      {
         if (a.size() > MAX_LEN || b.size() > MAX_LEN)
            throw out_of_range("Exceeding maximum allowed word length");

         // the 3rd parameter (the state) of the DP matrix
         enum op : uint8_t { MATCH_OR_SUBST, INSERT, DELETE };

         // to reuse the same 50x50 every time or at least comment it?
         uint8_t dp[MAX_LEN + 1][MAX_LEN + 1][3]; // VLA or MAX_LEN ?

#ifdef DEBUG
         uint8_t parent[a.size() + 1][b.size() + 1][3]{INF}; // to backtrack the operations and positions
#endif

         memset(dp, INF, sizeof(dp));
         dp[0][0][MATCH_OR_SUBST] = 0;

         // dist between "" and b[0]: {0,1,0}
         if (!b.empty())
            dp[0][1][INSERT] = 1;

         // dist between a[0] and "": {0,0,1}
         if (!a.empty())
            dp[1][0][DELETE] = 1;

         const int a_size = a.size();
         const int b_size = b.size();

         for (int i = 1; i <= a_size; ++i)
         {
            for (int j = 1; j <= b_size; ++j)
            {
               // subst costs 2: delete + insert
               uint8_t cost = (matchPolicy(a[i - 1], b[j - 1])) ? 0 : 2;

               // prev state can be anything
               uint8_t subs0 = dp[i - 1][j - 1][MATCH_OR_SUBST] + cost;
               uint8_t ins0 = dp[i - 1][j - 1][INSERT] + cost;
               uint8_t del0 = dp[i - 1][j - 1][DELETE] + cost;
               dp[i][j][MATCH_OR_SUBST] = min({subs0, ins0, del0});

               // prev state can not be INSERT
               uint8_t subs1 = dp[i][j - 1][MATCH_OR_SUBST] + 1;
               uint8_t del1 = dp[i][j - 1][DELETE] + 1;
               dp[i][j][INSERT] = min(subs1, del1);

               // prev state can not be DELETE
               uint8_t subs2 = dp[i - 1][j][MATCH_OR_SUBST] + 1;
               uint8_t ins2 = dp[i - 1][j][INSERT] + 1;
               dp[i][j][DELETE] = min(subs2, ins2);
            }
         }

         int distance = min({dp[a_size][b_size][MATCH_OR_SUBST], dp[a_size][b_size][INSERT], dp[a_size][b_size][DELETE]});
         return distance > 2 ? -1 : distance;
      }
   } // namespace

   int dist(string_view a, string_view b, CharMatchPolicy matcher)
   {
      using std::visit;
      // std::visit must not be called inside the implementation of distImpl, otherwise it would be called for every character
      return visit([&](const auto& policy) { return distImpl(a, b, policy); }, matcher);
   }
} // namespace spell::algorithm
