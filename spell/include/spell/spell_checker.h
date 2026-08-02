#pragma once

#include <string>
#include <string_view>
#include <unordered_set>

namespace spell_checker
{
   using std::string;
   using std::string_view;
   using std::unordered_set;

   string check(const unordered_set<string_view>& voc, string_view w);

   class SpellChecker
   {
   public:
      static string check(string_view s, char sep = ' ', string_view = "===");
   };
} // namespace spell_checker
