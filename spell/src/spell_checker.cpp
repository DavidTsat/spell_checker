#include "spell/spell_checker.h"
#include "spell/text_utils.h"
#include "spell/algorithm.h"

#ifdef DEBUG
#include "spell/debug_utils.h"
#endif

namespace spell_checker
{
   using namespace spell_checker::text;
   using namespace spell_checker::algorithm;

   string check(const unordered_set<string_view>& voc, string_view w)
   {
      // exact match
      if (voc.find(w) != voc.cend())
         return string{w};

      vector<string_view> d1, d2;
      for (string_view v : voc)
      {
         // if the length diff is > 2, we won't have any match
         // as we know the length is not longer than 50, we can cast to int
         if (abs((int)v.size() - (int)w.size()) > 2)
            continue;

         int d = dist(w, v);
         if (d > 2)
            continue;

         if (d == 1)
            d1.push_back(v);
         else
            d2.push_back(v);
      }

      string res = format(d1, d2);
      if (res.empty())
         return '{' + string(w) + "?}"; // no match

      return res;
   }

   string SpellChecker::check(string_view s, char sep, string_view term)
   {
      string r;
      // add homogenous lookup if c++20
      unordered_set<string_view> voc;
      vector<string_view> words;

      auto it = tokenize(s.cbegin(), s.cend(), inserter(voc, voc.begin()), sep, term);
      auto endIt = tokenize(it, s.cend(), inserter(words, words.begin()), sep, term);
      // todo: ret value checks

      // todo
      for (auto w : words)
         r += spell_checker::check(voc, w) + ' ';

      return r;
   }
} // namespace spell_checker
