A basic spell checker library and a client application.

The library (built as a static library libspell.a) exposes a custom distance function which between given two strings. The basic idea of the algorith is based on the distance Levenshtein https://en.wikipedia.org/wiki/Levenshtein_distance (the weight of operations: insertion 1, deletion 1, substitution 2).     
Allowed operations: insertion, deletion, substitution (subtitution is counted as 2 operations: deletion + insertion).    
Two consecutive insertions or deletions are not allowed.
The result is -1 if the distance is greater than 2.
Throws std::out_of_range if the length of the words is greater than MAX_LEN (50).

The algorithm is based on Lavenshtein distance, but with the additional constraint that two consecutive insertions or deletions are not allowed.
To fullfill this constraing, the algorithm uses a 3D dynamic programming matrix, where the third dimension represents the last operation performed (match/substitution, insertion, deletion).
The 3-rd dimension being a small state machine, doesn't allow transitions from insertion to insertion or deletion to deletion, thus preventing two consecutive insertions or deletions.
The time complexity is O(n*m) (strinctly speaking O(n*m*3)), where n and m are the lengths of the. A drawback is that the algorithm doesn't have an early stop,
that is, even if the distance is already greater than 2, it will continue to compute the entire matrix and return -1 at the end. This can be optimized.
