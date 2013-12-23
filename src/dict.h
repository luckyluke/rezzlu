
#ifndef DICT_H
#define DICT_H

#define __DEF_DICT_LEN 1000
#define WDICT_ROOT (char)0xff


struct wlist{
  char* word;
  struct wlist* next;
};

/* node of the optimized tree dictionary structure,
 * it represents a single char following a given word*/
struct wdict{
  /* char of the node */
  char ch;
  /* is this char the last of a valid word? */
  short int end;
  /* pointers to the next nodes in the tree */
  struct wdict** next;
  int _n_chars;
};

typedef struct {
  /* number of chars in the alphabet */
  int n_chars;
  /* TODO: list of chars in alphabet? */
  char* chars;
  /* TODO: statistics of frequency for each char */
  float* stats;
  /* number of words in the dict */
  int dlen;
  /* TODO: raw dict, i.e. list of words */
  struct wlist* dlist;
  /* struttura ottimizzata ad albero? */
  struct wdict* dict;
} dict_t;

dict_t* load_dict(const char* fname);
void free_dict(dict_t* d);
int lookup_dict(dict_t* d, const char* word);
void free_dict(dict_t* d);
void print_dict(dict_t* d);

typedef void (*word_f)(char*);
void walk_dict(dict_t* d, word_f f);

#endif /* DICT_H */
