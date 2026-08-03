#pragma once
#include "spell/text_utils.h"

#include <string>
#include <string_view>
#include <unordered_set>
#include <memory>

namespace spell_checker
{
   using namespace spell::text;

   using std::string;
   using std::string_view;
   using std::unique_ptr;
   using std::unordered_set;

   string check(const unordered_set<string_view>& lookup, const vector<string_view>& vocabulary, string_view word);

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
