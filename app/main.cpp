#include "spell_checker.h"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

using namespace spell_checker;

int main()
{
   SpellChecker checker{"rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
                       "mainy    oon teh lain was hints pliant==="};

   std::cout << checker.check() << std::endl;

   return 0;
}
