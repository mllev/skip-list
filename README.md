### skip-list
A high performance, header only skip list.

A dictionary that maps byte arrays (`unsigned char *`) to word values (`unsigned long`).

### Random Height
This implementation makes 3 optimizations to the standard skip list probability algorithm.
* The total "coin flip" results are generated before each insert. Rather than generating a 1 or a 0 by calling `rand()` on every iteration of the while loop (in the worst case calling `rand()` up to 32 times per insert), the algorithm is called once, and the bits of the result are used to determine the height. 
* `rand()` has been replaced with a simple string hash, `dbj2()`. The reason for this is because it has proven to provide a better distribution of 1s and 0s for the coin flip test, resulting in a better weighted more predictable probability. This leads to faster searches, and less memory usage given the more appropriate frequency of larger heights.
*  `dbj2()` is actually called only once every 20 or so inserts. This is achieved by caching the bits of the result, and only regenerating them once they've run out. The string that is hashed is the current key at the time of height generation.