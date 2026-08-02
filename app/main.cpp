#include "spell/spell_checker.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

using namespace spell_checker;

int main()
{
   SpellChecker sp;

   string_view testInp{"rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
                       "mainy    oon teh lain was hints pliant==="};

   /*
   cout << dist("hte", "the") << endl;
   cout << dist("the", "hte") << endl;
   cout << dist("abc", "abcde") << endl;
   cout << dist("abcde", "abc") << endl;
   cout << dist("abcl", "abcool") << endl;
   cout << dist("abcool", "abcl") << endl;
   cout << dist("abcool", "abcl") << endl;
   cout << dist("abcool", "abcl") << endl;
   cout << dist("abcool", "abcul") << endl;
   cout << dist("abcool", "abcukl") << endl;
   cout << dist("abcol", "abcul") << endl;
   cout << dist("abcol", "acl") << endl;
   */

   string s = sp.check(testInp);

   std::cout << s << std::endl;

   return 0;
}
