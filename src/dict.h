
#ifndef DICT_H
#define DICT_H

#define __DEF_DICT_LEN 1000
#define WDICT_ROOT (char)0xff


struct wlist{
  char* word;
  struct wlist* next;
};

struct wdict{
  char ch;
  short int end;
  struct wdict** next;
  int _n_chars;
};

typedef struct {
  int n_chars;
  char* chars;
  float* stats;
  int dlen;
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