#pragma once
#include "spell/text_utils.h"

#include <string>
#include <string_view>
#include <memory>

namespace spell_checker
{
   using namespace std;

   class SpellChecker
   {
   public:
      explicit SpellChecker(string_view inputSeq, char sep = ' ', string_view term = "===");
      ~SpellChecker();

      string check() const;

   private:
      class SpellCheckerImpl;
      unique_ptr<SpellCheckerImpl> pImpl;
   };
} // namespace spell_checker
