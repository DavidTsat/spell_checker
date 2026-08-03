A basic spell checker library and a client application.

The library (built as a static library libspell.a) exposes a custom distance function between given two string arguments (and an optional policy either case sensitive matching, or not, by default the comparison between individual characters are case insensitive).
Allowed operations: insertion, deletion, substitution (subtitution is counted as 2 operations: deletion + insertion).    
Two consecutive insertions or deletions are not allowed.
The result is -1 if the distance is greater than 2.
Throws std::out_of_range if the length of the words is greater than MAX_LEN (50).

The algorithm's core concept is based on Lavenshtein distance, but with the additional constraint that two consecutive insertions or deletions are not allowed.
To fullfill this constraint, the algorithm uses a 3D dynamic programming matrix, where the third dimension represents the last operation performed (match/substitution, insertion, deletion).
The 3-rd dimension being a small state machine, doesn't allow transitions from insertion to insertion or deletion to deletion, thus preventing two consecutive insertions or deletions.
The time complexity is O(n*m) (strictly speaking O(n*m*3)), where n and m are the lengths of the. A drawback is that the algorithm doesn't have an early stop,
that is, even if the distance is already greater than 2, it will continue to compute the entire matrix and return -1 at the end. This can be optimized.

The structure is as follows:
├── app
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── spell_checker.cpp
│   └── spell_checker.h
├── CMakeLists.txt
├── README.md
├── spell
│   ├── CMakeLists.txt
│   ├── include
│   │   └── spell
│   │       ├── algorithm.h
│   │       ├── debug_utils.h
│   │       └── text_utils.h
│   └── src
│       └── algorithm.cpp
└── test
    ├── app_test
    │   ├── CMakeLists.txt
    │   └── test.cpp
    ├── CMakeLists.txt
    └── lib_test
        ├── CMakeLists.txt
        └── test.cpp
        
The library exposes also the following functions from spell::debug_utils namespace:
1.
   template <typename InIt>
   string format(InIt beginD1, InIt endD1, InIt beginD2, InIt endD2, char sep)
Given two pair of begin and end iterators and a char separator the function iterates over the given sequences and produces a string output:
If the first sequence is non-empty, the second one is ignored, otherwise the second one only is used. 
If only a single element is contained in the chosen sequence, the output is padded with single opening and closing {} brackets, e.g. "{output}".

   template <typename InIt, typename OutIt>
   InIt tokenize(InIt first, InIt last, OutIt dest, char sep, string_view term)
Given an input sequence, and an output iterator, the tokenize function iterates over the input sequence, separated by the "sep" character, splits into single words and fills the output iterator until the terminal sequence "term" is reached or the input sequence is exhausted (iterated till "last").

The library code is under "spell".
The application code is placed under "app". Application is built into two part: "libspell_app_lib.a" static library, consisting of the orchestrator class spell_checker and an executable application spell_app, depending on the libspell_app_lib.

Example usage (can be found in app/main.cpp):
SpellChecker checker{"rain spain plain plaint pain main mainly the in on fall falls his was===hte rame in pain fells "
                       "mainy    oon teh lain was hints pliant==="};
                       
checker.check(); must produce "the {rame?} in pain falls {main mainly}    on the plain was {hints?} plaint". 

There are two more components: app_test and lib_test for unit tests (to migrate to gtests) under "./test". Both are built as (independent) executables and must be used and enhanced after each and every change to validate the expected results.

To build the whole project please do:
mkdir build &&
cd build &&
cmake .. &&
cmake --build . --clean-first"

Separate components can be built separately by:
cmake --build . --clean-first --target spell
cmake --build . --clean-first --target spell_app
cmake --build . --clean-first --target spell_lib_tests
cmake --build . --clean-first --target spell_app_tests

After building please execute the unit tests (from the build directory) later:
./test/app_test/spell_app_tests
and
./test/lib_test/spell_lib_tests
You should see "All ... tests passed!" on each line.
The application can be started by:
./app/spell_app.

To test new input and output main.cpp should be changed and the application must be rebuilt: "cmake --build . --clean-first --target spell_app".
