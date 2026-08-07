#pragma once
#include "spell/text_utils.h"

#include <string>
#include <string_view>
#include <memory>
#include <fstream>

namespace spell_checker
{
   class SpellChecker
   {
   public:
      SpellChecker(const SpellChecker&) = delete;
      SpellChecker(SpellChecker&&) = delete;

      SpellChecker& operator=(const SpellChecker&) = delete;
      SpellChecker& operator=(SpellChecker&&) = delete;

      explicit SpellChecker(std::string_view inputSeq, char sep = ' ', std::string_view term = "===");
      explicit SpellChecker(std::ifstream& instream, char sep = ' ', std::string_view term = "===");

      ~SpellChecker();

      std::string check() const;

   private:
      template <typename CharMatchPolicy>
      class SpellCheckerImpl;

      std::unique_ptr<SpellCheckerImpl<spell::text::CaseInsensitiveMatch>> pImpl;
   };
} // namespace spell_checker
