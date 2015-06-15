/*
 * A high performance, header only skip list
 * Author - Matthew Levenstein
 * License - MIT
 */

#ifndef _LIST_H
#define _LIST_H

#include <string.h>
#include <stdlib.h>

#define LIST_MAX_HEIGHT 32

typedef unsigned char   byte_t;
typedef unsigned long   word_t;

typedef struct listNode listNode;

word_t bytes;

struct listNode {
  byte_t*   key;
  int       klen;
  byte_t    height;
  word_t    val;
  listNode* next[1];
};

typedef struct {
  byte_t    height;
  int       bits;
  int       reset;
  listNode* head;
} list;

#define _DBJ2(s, l, z) do { \
  unsigned i; \
  for (i = 0; i < l; i++) \
    z = ((z << 5) + z) + s[i]; \
} while (0)

#define LIST_MALLOC(e, s) do { \
  e = malloc(s); \
  if (!e) { \
    fprintf(stderr, "Fatal: out of memory."); \
    abort(); \
  } \
  bytes += s; \
  memset(e, 0, s); \
} while (0)

#define LIST_HEIGHT(d, str, l, z) do { \
  int f = 0; \
  for (z = 0; !f; z++) { \
    if (!d->reset) { \
      d->bits = 5381; \
      _DBJ2(str, l, d->bits); \
      d->reset = LIST_MAX_HEIGHT; \
    } \
    f = d->bits & 1;\
    d->bits = d->bits >> 1; \
    --(d->reset); \
  } \
} while (0)

#define LIST_INIT(d) do { \
  LIST_MALLOC(d, sizeof(list)); \
  LIST_MALLOC(d->head, (sizeof(listNode)+ \
    ((sizeof(listNode*))*(LIST_MAX_HEIGHT - 1)))); \
  d->height = 0; \
} while (0)

#define LIST_GET(d, k, l, v) do { \
  LIST_FIND(d, k, l, v, NULL); \
} while (0)

#define LIST_FIND(d, k, l, v, u) do { \
  listNode *itr = d->head, **ud = u, *n; \
  int i = d->height - 1; \
  for (; i >= 0; --i) { \
    while (itr->next[i] && memcmp(k, itr->next[i]->key, l) > 0) \
      itr = itr->next[i]; \
    if (ud) ud[i] = itr; \
  } \
  n = itr->next[0]; \
  if (n && l == n->klen && !memcmp(k, n->key, l)) \
    v = n->val; \
  else v = 0; \
} while (0)

#define LIST_NODE_INIT(n, k, l, v, h) do { \
  LIST_MALLOC(n, (sizeof(listNode)+ \
    ((sizeof(listNode*))*(h-1)))); \
  LIST_MALLOC(n->key, l + 1); \
  memcpy(n->key, k, l); \
  n->val = (word_t)v; \
  n->height = h; \
  n->klen = l; \
} while (0)

#define LIST_INSERT(d, k, l, v) do { \
  int i, h; \
  word_t vl; \
  listNode *n, *u[LIST_MAX_HEIGHT]; \
  LIST_FIND(d, k, l, vl, u); \
  if (!vl) { \
    LIST_HEIGHT(d, k, l, h); \
    if (h > d->height) { \
      h = ++(d->height); \
      u[h - 1] = d->head; \
    } \
    LIST_NODE_INIT(n, k, l, v, h); \
    while (--h >= 0) { \
      n->next[h] = u[h]->next[h]; \
      u[h]->next[h] = n; \
    } \
  } \
} while (0)

#define LIST_NODE_DESTROY(n) do { \
  free(((listNode *)n)->key); \
  free((listNode *)n); \
} while (0)

#define LIST_DELETE(d, k, l) do { \
  int i, h; \
  word_t vl; \
  listNode *n, *u[LIST_MAX_HEIGHT]; \
  LIST_FIND(d, k, l, vl, u); \
  if (vl) { \
    for (i = 0; i < d->height; i++) { \
      if (u[i]->next[i]) \
        u[i]->next[i] = u[i]->next[i]->next[i]; \
    } \
    while (d->height) { \
      if (d->head->next[d->height - 1]) \
        break; \
      d->head->next[--(d->height)] = NULL; \
    } \
    LIST_NODE_DESTROY(n); \
  } \
} while (0)

#define LIST_ITERATE(d, it) do { \
  listNode *itr; \
  itr = d->head; \
  while (itr->next[0]) { \
    itr = itr->next[0]; \
    it(itr); \
  } \
} while (0)

#endif
