#include "spell_checker.h"
#include "spell/algorithm.h"

#include <iterator>
#include <vector>
#include <unordered_set>
#include <string>

#ifdef DEBUG
#include "spell/debug_utils.h"
#endif

namespace spell_checker
{
   using namespace spell::algorithm;
   using namespace spell::text;

   template <typename LookupCont, typename Policy>
   string checkImpl(const LookupCont& lookup, const vector<string_view>& vocabulary, string_view prefix,
                    string_view word, Policy policy, char sep)
   {
      // fast path, exact match
      if (lookup.find(word) != lookup.cend())
         return string(prefix) + string(word);

      vector<string_view> d1, d2;
      for (string_view vocWord : vocabulary)
      {
         if (abs((int)vocWord.size() - (int)word.size()) > 2)
            continue;

         int d = dist(vocWord, word, policy);
         if (d == -1)
            continue;

         if (d == 1)
            d1.push_back(vocWord);
         else if (d == 2)
            d2.push_back(vocWord);
      }

      string res{format(d1.cbegin(), d1.cend(), d2.cbegin(), d2.cend(), sep)};

      if (res.empty())
         res = '{' + string(word) + "?}"; // no match

      return string(prefix) + string(res);
   }

   SpellChecker::SpellChecker(string_view inputSeq, char sep, string_view term) :
      pImpl{make_unique<SpellCheckerImpl<CaseInsensitiveMatch>>(inputSeq, sep, term)}
   {}

   SpellChecker::SpellChecker(ifstream& instream, char sep, string_view term) :
      pImpl{make_unique<SpellCheckerImpl<CaseInsensitiveMatch>>(instream, sep, term)}
   {}

   SpellChecker::~SpellChecker() = default;

   template <typename CharMatchPolicy>
   class SpellChecker::SpellCheckerImpl
   {
   public:
      SpellCheckerImpl(const SpellCheckerImpl&) = delete;
      SpellCheckerImpl(SpellCheckerImpl&&) = delete;

      SpellCheckerImpl& operator=(const SpellCheckerImpl&) = delete;
      SpellCheckerImpl& operator=(SpellCheckerImpl&&) = delete;

      explicit SpellCheckerImpl(string_view inputSeq, char sep, string_view term) :
         mSep(sep)
      {
         initData(inputSeq.cbegin(), inputSeq.cend(), term);
      }

      explicit SpellCheckerImpl(ifstream& infile, char sep, string_view term) :
         mSep(sep)
      {
         if (!infile.is_open())
            throw runtime_error("File error!");

         mFileBuffer.assign(istreambuf_iterator<char>(infile), istreambuf_iterator<char>());
         initData(mFileBuffer.cbegin(), mFileBuffer.cend(), term);
      }

      string check() const
      {
         if (mWords.empty())
            return {};

         string r;
         for (auto it = mWords.cbegin(); it != mWords.cend(); ++it)
            r += checkImpl(mVocLookup, mVocLinear, it->second, it->first, CharMatchPolicy{}, mSep);

         return r;
      }

   private:
      template <typename It>
      void initData(It first, It last, std::string_view term)
      {
         auto it = tokenize<false>(first, last, std::back_inserter(mVocLinear), term, true);

         // Skip a single padding space right after term, if present
         if (it != last && isspace(static_cast<unsigned char>(*it)))
            ++it;

         tokenize<true>(it, last, std::back_inserter(mWords), term, false);

         mVocLookup.reserve(mVocLinear.size());
         mVocLookup.insert(mVocLinear.cbegin(), mVocLinear.cend());
      }

      char mSep;
      string mFileBuffer;
      vector<string_view> mVocLinear;                // to keep the original order of the vocabulary
      vector<pair<string_view, string_view>> mWords; // word and its prefix
      unordered_set<string_view, CharMatchPolicy, CharMatchPolicy> mVocLookup; // for fast lookup for exact matches
   };

   string SpellChecker::check() const
   {
      return pImpl->check();
   }
} // namespace spell_checker
