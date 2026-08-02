#include "spell/spell_checker.h"
#include "spell/algorithm.h"
#include "spell/text_utils.h"

#include <iostream>
#include <cassert>

namespace
{
   using namespace spell_checker;
   using namespace spell_checker::algorithm;
   using namespace spell_checker::text;

   using std::cout;

   void testDist()
   {
      assert(dist("hte", "the") == 2);        // one deletion + one insertion (non consecutive)
      assert(dist("the", "hte") == 2);        // must be symmetric
      assert(dist("abc", "abcde") == -1);     // two consecutive insertions, not allowed
      assert(dist("abcde", "abc") == -1);     // two consecutive deletions, not allowed
      assert(dist("abcl", "abcool") == -1);   // two consecutive insertions, not allowed
      assert(dist("abcool", "abcl") == -1);   // two consecutive deletions, not allowed
      assert(dist("abcool", "abcul") == -1);  // delete + insert + insert = 3 operations, not allowed
      assert(dist("abcool", "abcukl") == -1); // two substitutions = 4 operations (result -1), not allowed
      assert(dist("abcol", "abcul") == 2);    // one substitution = one deletion + one insertion
      assert(dist("abcol", "acl") == 2);      // two non consecutive deletions
      assert(dist("acl", "abcol") == 2);      // two non consecutive insertions

      assert(dist("", "") == 0); // empty strings must have zero distance
      assert(dist("01234567890123456789012345678901234567890123456789",
                  "01234567890123456789012345678901234567890123456789")
             == 0); // 50 is the maximum length, must have zero distance
      assert(dist("01234567890123456789012345678901234567890123456789",
                  "0123456789012345678901234567890123456789012345678")
             == 1); // 50 vs 49, must have distance 1
      assert(dist("01234567890123456789012345678901234567890123456789",
                  "0123456789012345678901234567890123456789012345678a")
             == 2); // 50 vs 50, must have distance 2

      try
      {
         dist("01234567890123456789012345678901234567890123456789",
              "012345678901234567890123456789012345678901234567890"); // 50 vs 51, must throw exception
         assert(false);                                               // should not reach here
      }
      catch (const std::out_of_range& e)
      {
         assert(true); // expected exception
      }

      // tests with mixed case
      assert(dist("Abc", "abc") == 0);      // the algorithm is case insensitive
      assert(dist("abc", "Abc") == 0);      // check the symmetry
      assert(dist("HeLlO", "helloU") == 1); // check the symmetry

      // some more tests
      assert(dist("", "01234567890123456789012345678901234567890123456789") == -1); // min size word vs max size word
      assert(dist("01234567890123456789012345678901234567890123456789", "") == -1); // the symmetric case
      assert(dist("98765432109876543210987654321098765432109876543210",
                  "98765432109876543210987654321098765432109876543210")
             == -1); // all characters different, but the algorithm must compute till the end
      assert(dist("98765432109876543210987654321098765432109876543210",
                  "98765432109876543210987654321098765432109876543210")
             == -1); // the symmetric case

      // Latin-1/ISO 8859-1 (extended ASCII) strings
      assert(dist("München", "Munchen") == 2);         // one substitution
      assert(dist("Munchen", "München") == 2);         // the symmetric case
      assert(dist("ßäöüÂÔáàÊÙÀ", "ßäöüÂÔáàÊÙÀ") == 0); // German umlauts

      // byte strings (non-ASCII)
      assert(dist("\xC3\xA4\xE4\xF6\xFC", "\xC3\xA4\xE4\xF6\xFC") == 0);
      assert(dist("\xC3\xA4", "\xC3\xB6") == 2);
      assert(dist("\xC3\xB6", "\xC3\xA4") == 2);

      cout << "All dist tests passed!\n";
   }
} // namespace

// change it to gtests
int main()
{
   testDist();

   return 0;
}
