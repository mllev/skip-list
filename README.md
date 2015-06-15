### Skip List
A high performance, header only skip list.

A dictionary that maps byte arrays (`unsigned char *`) to word values (`unsigned long`).

### Random Height
This implementation makes 3 optimizations to the standard skip list probability algorithm.
* The total "coin flip" results are generated before each insert. Rather than generating a 1 or a 0 by calling `rand()` on every iteration of the while loop (in the worst case calling `rand()` up to 32 times per insert), the algorithm is called once, and the bits of the result are used to determine the height. 
* `rand()` has been replaced with a simple string hash, `dbj2()`. The reason for this is because it has proven to provide a better distribution of 1s and 0s for the coin flip test, resulting in a better weighted more predictable probability. This leads to faster searches, and less memory usage given the more appropriate frequency of larger heights.
*  `dbj2()` is actually called only once every 20 or so inserts. This is achieved by caching the bits of the result, and only regenerating them once they've run out. The string that is hashed is the current key at the time of height generation.

### Usage

```c
#include <stdio.h>

#include "list.h"

int main (void) {
  list* d;
  unsigned char *key, *val;
  unsigned long v;
  int l;

  /* initialize the list */
  LIST_INIT(d);

  /* create a key */
  key = malloc(6);
  strncpy(key, "hello", 5);
  key[5] = 0;

  l = 5;

  /* create a value */
  val = malloc(6);
  strncpy(val, "world", 5);
  val[5] = 0;

  /* insert into the list */
  LIST_INSERT(d, key, l, (unsigned long)val);

  /* retrieve from a list */
  LIST_GET(d, key, l, v);

  printf("Hello, %s!\n", (char *)v);
  return 0;
}
```