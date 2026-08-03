#include "spell_checker.h"

#include <string_view>
#include <iostream>
#include <cassert>

namespace
{
   using namespace spell_checker;

   using std::cout;
   using std::string;
   using std::string_view;

   void spellCheckerTest()
   {
      struct SpellCheckerBasicTests
      {
         void operator()(string_view input, string_view expRes)
         {
            SpellChecker checker{input};

            assert(checker.check() == expRes);
         }
      };

      SpellCheckerBasicTests test;

      test("rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
           "mainy    oon teh lain was hints pliant===",
           "the {rame?} in pain falls {main mainly}    on the plain was {hints?} plaint");

      test("in on===mainy        oon===", "{mainy?}        on");

      test("a ab abc abcd abcde abcdef === abcdef a ab abc abcd abcde ===", "abcdef a ab abc abcd abcde");

      test("abcde abc abcdeg abcef === abcdef ae===", "{abcde abcef} {ae?}");

      test("hello world===                helo wrld", "               hello world");

      test("cat at ot ct mat co===cot dog", "{ot ct co} {dog?}");

      test("one two three===four five six", "{four?} {five?} {six?}");

      test("abc adc aec===afc", "{abc adc aec}");

      // empty dictionary
      test("===hello world", "{hello?} {world?}");

      // empty words
      test("hello world===", "");

      // max length
      test("01234567890123456789012345678901234567890123456789===0123456789012345678901234567890123456789012345678",
           "01234567890123456789012345678901234567890123456789");

      // separators (spaces) must be preserved
      test("rain plain pain===hte   rame   aaaaa", "{hte?} {  rame?} {  aaaaa?}");

      // several dist 2 candidates
      test("abcde abceq bbcee===abcee", "{abcde abceq bbcee}");

      // mixed
      test("rain brain train plain===rani brin plian", "rain brain plain");

      // duplicate vocabulary words
      test("the the the===teh", "{the the the}");

      test("abcdef === abcde ===", "abcdef");

      // both empty
      test("", "");

      // both empty with opening terminal sequence
      test("===", "");

      // both empty with opening and closing terminal sequence
      test("======", "");

      cout << "All spellChecker tests passed!\n";
   }

} // namespace

// change it to gtests
int main()
{
   spellCheckerTest();
   return 0;
}
