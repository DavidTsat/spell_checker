#include "spell/algorithm.h"
#include "spell/text_utils.h"

#include <cassert>
#include <vector>
#include <iostream>

namespace
{
   using namespace spell::algorithm;
   using namespace spell::text;

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

      // tests with mixed case; insensitive match policy
      assert(dist("Abc", "abc") == 0);      // the algorithm is case insensitive
      assert(dist("abc", "Abc") == 0);      // check the symmetry
      assert(dist("HeLlO", "helloU") == 1); // check the symmetry

      // tests with mixed case; sensitive match policy
      assert(dist("Abc", "abc", CaseSensitiveMatch{}) == 2);       // the algorithm is case sensitive
      assert(dist("abc", "Abc", CaseSensitiveMatch{}) == 2);       // check the symmetry
      assert(dist("HeLlO", "helloU", CaseSensitiveMatch{}) == -1); // check the symmetry

      // some more tests
      assert(dist("", "01234567890123456789012345678901234567890123456789") == -1); // min size word vs max size word
      assert(dist("01234567890123456789012345678901234567890123456789", "") == -1); // the symmetric case
      assert(dist("98765432109876543210987654321098765432109876543210",
                  "01234567890123456789012345678901234567890123456789")
             == -1); // all characters different, but the algorithm must compute till the end
      assert(dist("01234567890123456789012345678901234567890123456789",
                  "98765432109876543210987654321098765432109876543210")
   
             == -1); // the symmetric case

      assert(dist("aa", "aaaa") == 2);

      assert(dist("", "") == 0);
      assert(dist("", "k") == 1);
      assert(dist("", "kk") == -1);
      assert(dist("s", "skk") == -1);
      assert(dist("sp", "spkk") == -1);
      assert(dist("spa", "spakk") == -1);
      assert(dist("spai", "spaikk") == -1);
      assert(dist("spain", "spainkk") == -1);

      assert(dist("0123456", "012345655") == -1);
      assert(dist("01234567", "0123456755") == -1);
      assert(dist("012345678", "01234567855") == -1);
      assert(dist("0123456789", "012345678955") == -1);
      assert(dist("01234567890", "0123456789055") == -1);
      assert(dist("012345678901", "01234567890155") == -1);
      assert(dist("0123456789012", "012345678901255") == -1);
      assert(dist("01234567890123", "0123456789012355") == -1);
      assert(dist("012345678901234", "01234567890123455") == -1);
      assert(dist("0123456789012345", "012345678901234555") == 2); // this must be two as the insertions are not consecutive
      assert(dist("0123456789012344", "012345678901234455") == -1);
      assert(dist("01234567890123456", "0123456789012345655") == -1);
      assert(dist("012345678901234567", "01234567890123456755") == -1);
      assert(dist("0123456789012345678", "012345678901234567855") == -1);
      assert(dist("01234567890123456789", "0123456789012345678955") == -1);
      assert(dist("012345678901234567890", "01234567890123456789055") == -1);
      assert(dist("0123456789012345678901", "012345678901234567890155") == -1);
      assert(dist("01234567890123456789012", "0123456789012345678901255") == -1);
      assert(dist("012345678901234567890123", "01234567890123456789012355") == -1);
      assert(dist("0123456789012345678901234", "012345678901234567890123455") == -1);
      assert(dist("01234567890123456789012345", "0123456789012345678901234555") == 2); // the same as above
      assert(dist("01234567890123456789012344", "0123456789012345678901234455") == -1);
      assert(dist("012345678901234567890123456", "01234567890123456789012345655") == -1);
      assert(dist("0123456789012345678901234567", "012345678901234567890123456755") == -1);
      assert(dist("01234567890123456789012345678", "0123456789012345678901234567855") == -1);
      assert(dist("012345678901234567890123456789", "01234567890123456789012345678955") == -1);
      assert(dist("0123456789012345678901234567890", "012345678901234567890123456789055") == -1);
      assert(dist("01234567890123456789012345678901", "0123456789012345678901234567890155") == -1);
      assert(dist("012345678901234567890123456789012", "01234567890123456789012345678901255") == -1);
      assert(dist("0123456789012345678901234567890123", "012345678901234567890123456789012355") == -1);
      assert(dist("01234567890123456789012345678901234", "0123456789012345678901234567890123455") == -1);
      assert(dist("012345678901234567890123456789012345", "01234567890123456789012345678901234555") == 2); // the same as above
      assert(dist("012345678901234567890123456789012344", "01234567890123456789012345678901234455") == -1);
      assert(dist("0123456789012345678901234567890123456", "012345678901234567890123456789012345655") == -1);
      assert(dist("01234567890123456789012345678901234567", "0123456789012345678901234567890123456755") == -1);
      assert(dist("012345678901234567890123456789012345678", "01234567890123456789012345678901234567855") == -1);
      assert(dist("0123456789012345678901234567890123456789", "012345678901234567890123456789012345678955") == -1);
      assert(dist("01234567890123456789012345678901234567890", "0123456789012345678901234567890123456789055") == -1);
      assert(dist("012345678901234567890123456789012345678901", "01234567890123456789012345678901234567890155") == -1);
      assert(dist("0123456789012345678901234567890123456789012", "012345678901234567890123456789012345678901255") == -1);
      assert(dist("01234567890123456789012345678901234567890123", "0123456789012345678901234567890123456789012355") == -1);
      assert(dist("012345678901234567890123456789012345678901234", "01234567890123456789012345678901234567890123455") == -1);
      assert(dist("0123456789012345678901234567890123456789012345", "012345678901234567890123456789012345678901234555") == 2); // the same as above
      assert(dist("0123456789012345678901234567890123456789012344", "012345678901234567890123456789012345678901234455") == -1); 
      assert(dist("01234567890123456789012345678901234567890123456", "0123456789012345678901234567890123456789012345655") == -1);
      assert(dist("012345678901234567890123456789012345678901234567", "01234567890123456789012345678901234567890123456755") == -1);

      // TODO: the policy classes must be enhanced
      // Latin-1/ISO 8859-1 (extended ASCII) strings
      //   assert(dist("München", "Munchen") == 2);         // one substitution
      //   assert(dist("Munchen", "München") == 2);         // the symmetric case
      //   assert(dist("ßäöüÂÔáàÊÙÀ", "ßäöüÂÔáàÊÙÀ") == 0); // German umlauts

      //   // byte strings (non-ASCII)
      //   assert(dist("\xC3\xA4\xE4\xF6\xFC", "\xC3\xA4\xE4\xF6\xFC") == 0);
      //   assert(dist("\xC3\xA4", "\xC3\xB6") == 2);
      //   assert(dist("\xC3\xB6", "\xC3\xA4") == 2);

      cout << "All dist tests passed!\n";
   }

   void testPrePostProcessor()
   {
      auto testPrePostProcessorBasic =
         [](const auto& processor, string_view toPrep, string_view preprocessed, string_view postprocessed)
      {
         auto [prepOut, prefix] = processor.preprocess(toPrep);
         assert(prepOut == preprocessed);
         auto postOut = processor.postprocess(prefix, prepOut);
         assert(postOut == postprocessed);
      };

      LTrimWhitespace processor;
      testPrePostProcessorBasic(processor, "     abc", "abc", "     abc");
      testPrePostProcessorBasic(processor, "\tabc", "abc", "\tabc");
      testPrePostProcessorBasic(processor, "\nabc", "abc", "\nabc");
      testPrePostProcessorBasic(processor, "\rabc", "abc", "\rabc");
      testPrePostProcessorBasic(processor, "\fabc", "abc", "\fabc");
      testPrePostProcessorBasic(processor, "\vabc", "abc", "\vabc");
      testPrePostProcessorBasic(processor, " \t\n\r\f\vabc", "abc", " \t\n\r\f\vabc"); // mixed

      cout << "All pre and post processor tests passed!\n";
   }

   void testFormat()
   {
      auto testFormatBasic = [](auto beg1, auto end1, auto beg2, auto end2, const string& expectedOut)
      { assert(expectedOut == format(beg1, end1, beg2, end2, ' ')); };

      vector<string_view> v1({"abc", "cde", "aaaa"});
      vector<string_view> v2({"mmm", "qqq", "ttttt"});
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "{abc cde aaaa}"); // only the first one must be considered
      testFormatBasic(v2.cbegin(), v2.cend(), v1.cbegin(), v1.cend(), "{mmm qqq ttttt}"); // the opposite

      v1 = {};
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "{mmm qqq ttttt}");

      v1.swap(v2);
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "{mmm qqq ttttt}");

      v1 = {};
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "");

      v1 = {"aaaaa"};
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "aaaaa");

      v1.swap(v2);
      testFormatBasic(v1.cbegin(), v1.cend(), v2.cbegin(), v2.cend(), "aaaaa");
   }

   void testTokenize()
   {
      struct TestTokenizeBasic
      {
         void operator()(string_view input, string_view term, const vector<string_view>& expectedVoc,
                         const vector<string_view>& expectedWords)
         {
            vector<string_view> voc;
            vector<string_view> words;

            auto it = tokenize(input.cbegin(), input.cend(), back_inserter(voc), term, true);
            tokenize(it, input.cend(), back_inserter(words), term, true);

            assert(voc == expectedVoc);
            assert(words == expectedWords);
         }
      } testTokenizeBasic;

      testTokenizeBasic(
         "rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
         "mainy    oon teh lain was hints pliant===", "===",
         {"rain", "spain", "plain", "plaint", "pain", "main", "mainly", "the", "in", "on", "fall", "falls", "his", "was"},
         {"hte", "rame", "in", "pain", "fells", "mainy", "oon", "teh", "lain", "was", "hints", "pliant"});

      testTokenizeBasic(
         "rain\rspain\rplain\rplaint\rpain\rmain\rmainly\nthe\f\n\f\v in\ron fall falls his\twas------hte rame\rin pain\ffells\f"
         "mainy \n\r    oon\nteh\flain was\thints pliant------", "------",
         {"rain", "spain", "plain", "plaint", "pain", "main", "mainly", "the", "in", "on", "fall", "falls", "his", "was"},
         {"hte", "rame", "in", "pain", "fells", "mainy", "oon", "teh", "lain", "was", "hints", "pliant"});

      testTokenizeBasic("hello === ===", "===", {"hello"}, {});
      testTokenizeBasic("======", "===", {}, {});

      cout << "All tokenize tests passed!\n";
   }
} // namespace

// change it to gtests
int main()
{
   testDist();
   // testPrePostProcessor(); // to remove
   testFormat();
   testTokenize();

   return 0;
}
