#include "spell/spell_checker.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

using namespace spell_checker;

int main()
{
   SpellChecker checker{"rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
                       "mainy    oon teh lain was hints pliant==="};

   string s = checker.check();

   std::cout << s << std::endl;

   return 0;
}
