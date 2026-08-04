#include "spell_checker.h"
#include "spell/algorithm.h"

#include <vector>
#include <unordered_set>

#ifdef DEBUG
#include "spell/debug_utils.h"
#endif

namespace spell_checker
{
   using namespace spell::algorithm;
   using namespace spell::text;

   template <typename WordProcesor>
   string checkImpl(const unordered_set<string_view>& lookup, const vector<string_view>& vocabulary, string_view word,
                    WordProcesor processor, char sep)
   {
      // fast path, exact match
      if (lookup.find(word) != lookup.cend())
         return string{word};

      // to do: to optimize
      vector<string> d1, d2;
      for (string_view vocWord : vocabulary)
      {
         auto [processedWord, prefix] = processor.preprocess(word);

         // if the length diff is > 2, we won't have any match
         // as we know the length is not longer than 50, we can cast to int
         if (abs((int)vocWord.size() - (int)processedWord.size()) > 2)
            continue;

         int d = dist(vocWord, processedWord);
         if (d == -1)
            continue;

         string res{processor.postprocess(prefix, vocWord)};
         if (d == 1)
            d1.push_back(move(res));
         else // d == 2
            d2.push_back(move(res));
      }

      string res{format(d1.cbegin(), d1.cend(), d2.cbegin(), d2.cend(), sep)};
      if (res.empty())
         return '{' + string(word) + "?}"; // no match

      return res;
   }

   SpellChecker::SpellChecker(string_view inputSeq, char sep, string_view term) :
      pImpl{std::make_unique<SpellCheckerImpl>(inputSeq, sep, term)}
   {}

   SpellChecker::~SpellChecker() = default;

   class SpellChecker::SpellCheckerImpl
   {
   public:
      explicit SpellCheckerImpl(string_view inputSeq, char sep, string_view term) :
         mSep(sep)
      {
         auto it{tokenize(inputSeq.cbegin(), inputSeq.cend(), back_inserter(mVocLinear), mSep, term)};
         auto endIt{tokenize(it, inputSeq.cend(), back_inserter(mWords), mSep, term)};

#ifdef DEBUG
         // todo
#endif

         // todo: to check the returned values
         mVocLookup.reserve(mVocLinear.size());
         mVocLookup.insert(mVocLinear.cbegin(), mVocLinear.cend());
      }

      string check() const
      {
         string r;
         for (auto it = mWords.cbegin(); it != mWords.cend(); ++it)
         {
            r += checkImpl<LTrimWhitespace>(mVocLookup, mVocLinear, *it, LTrimWhitespace{}, mSep);
            if (it != prev(mWords.cend()))
               r += mSep;
         }

         return r;
      }

   private:
      char mSep;
      vector<string_view> mVocLinear;        // to keep the original order of the vocabulary
      vector<string_view> mWords;            // to check the iterators invalidation
      unordered_set<string_view> mVocLookup; // for fast lookup for exact matches
   };

   string SpellChecker::check() const
   {
      return pImpl->check();
   }
} // namespace spell_checker
