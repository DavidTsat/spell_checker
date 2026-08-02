#include "spell/spell_checker.h"
#include "spell/algorithm.h"

#ifdef DEBUG
#include "spell/debug_utils.h"
#endif

// maybe move to the application layer, as this is not part of the library ?
namespace spell_checker
{
   using namespace spell_checker::algorithm;

   string check(const unordered_set<string_view>& lookup, const vector<string_view>& vocabulary, string_view word, const CharMatchPolicy& charMatchPolicy)
   {
      // fast path, exact match
      if (lookup.find(word) != lookup.cend())
         return string{word};

      vector<string_view> d1, d2;
      for (string_view v : vocabulary)
      {
         // if the length diff is > 2, we won't have any match
         // as we know the length is not longer than 50, we can cast to int
         if (abs((int)v.size() - (int)word.size()) > 2)
            continue;

         int d = dist(word, v, charMatchPolicy);
         if (d == -1)
            continue;

         if (d == 1)
            d1.push_back(v);
         else // d == 2
            d2.push_back(v);
      }

      string res = format(d1, d2);
      if (res.empty())
         return '{' + string(word) + "?}"; // no match

      return res;
   }

   SpellChecker::SpellChecker(string_view inputSeq, char sep, string_view term, CharMatchPolicy charMatchPolicy) :
         pImpl{std::make_unique<SpellCheckerImpl>(inputSeq, sep, term, std::move(charMatchPolicy))}
      {}

   class SpellChecker::SpellCheckerImpl
   {
   public:
      explicit SpellCheckerImpl(string_view inputSeq, char sep, string_view term,CharMatchPolicy&& charMatchPolicy) :
         charMatchPolicy(std::move(charMatchPolicy))
      {
         auto it = tokenize(inputSeq.cbegin(), inputSeq.cend(), inserter(vocLinear, vocLinear.begin()), sep, term);
         auto endIt = tokenize(it, inputSeq.cend(), inserter(words, words.begin()), sep, term);
         // assert
         // todo: to check the returned values
         vocLookup.insert(vocLinear.cbegin(), vocLinear.cend());
      }

      string check()
      {
         string r;

         // todo
         for (auto w : words)
            r += spell_checker::check(vocLookup, vocLinear, w, charMatchPolicy) + ' ';

         return r;
      }

   private:
      CharMatchPolicy charMatchPolicy;

      vector<string_view> vocLinear;        // to keep the original order of the vocabulary
      vector<string_view> words;            // to check the iterators invalidation
      unordered_set<string_view> vocLookup; // for fast lookup for exact matches
   };

   string SpellChecker::check()
   {
      return pImpl->check();
   }
} // namespace spell_checker
