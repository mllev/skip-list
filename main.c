#include <stdio.h>
#include <time.h>

#include "list.h"

byte_t* getWord (FILE* f) {
  byte_t* buf = NULL;
  int c = 0, i = 0, bufsize = 10;
  buf = malloc(bufsize + 1);
  memset(buf, 0, bufsize + 1);
  while ((c = fgetc(f)) != '\n') {
    if (c == EOF) return NULL;
    if (i == bufsize)
      buf = realloc(buf, (bufsize += 10) + 1);
    buf[i++] = (byte_t)c;
  }
  buf[i] = 0;
  return buf;
} 

void loadWords (list* root) {
  FILE* in = fopen("words.txt", "r");
  byte_t* word = NULL;

  /* read one word in at a time from the file */
  while ((word = getWord(in))) {
    int l = strlen((char *)word);
    word_t val = 0;

    /* insert into the list */
    LIST_INSERT(root, word, l, val);

    free(word);
    word = NULL;
  }
}

void printKey (listNode* n) {
  puts((const char *)n->key);
}

/* this tests the sorting speed of ~100,000 words */
int main (int argc, char** argv) {
  list* d;
  word_t v;
  double end, start;

  /* initialize the list */
  LIST_INIT(d);

  start = (float)clock()/CLOCKS_PER_SEC;
  /* load 100,000 english words into the structure */
  loadWords(d);
  /* iterate through the list and print */
  LIST_ITERATE(d, printKey);

  end = (float)clock()/CLOCKS_PER_SEC;
  printf("Memory used: %lu bytes.\n", bytes);
  printf("Finshed in %f.\n", end-start);

  return 0;
}
