#pragma once
#include "spell/text_utils.h"

#include <string>
#include <string_view>
#include <memory>
#include <fstream>

namespace spell_checker
{
   using namespace std;

   class SpellChecker
   {
   public:
      SpellChecker(const SpellChecker&) = delete;
      SpellChecker(SpellChecker&&) = delete;

      SpellChecker& operator=(const SpellChecker&) = delete;
      SpellChecker& operator=(SpellChecker&&) = delete;

      explicit SpellChecker(string_view inputSeq, char sep = ' ', string_view term = "===");
      explicit SpellChecker(ifstream& instream, char sep = ' ', string_view term = "===");

      ~SpellChecker();

      string check() const;

   private:
      class SpellCheckerImpl;
      unique_ptr<SpellCheckerImpl> pImpl;
   };
} // namespace spell_checker
