#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "dict.h"

struct wdict* wdict_alloc(char ch, int n_chars){
  struct wdict* tmp;
  if ((tmp = malloc(sizeof(struct wdict))) == NULL){
    perror("alloc wdict");
    return NULL;
  }
  tmp->ch = ch;
  tmp->_n_chars = n_chars;
  if ((tmp->next = malloc(sizeof(struct wdict*)*n_chars)) == NULL){
    free(tmp);
    perror("alloc wdict 2");
    return NULL;
  }
  return tmp;
}

void wdict_free(struct wdict* wd){
  int i;

  for (i=0; i<wd->_n_chars; i++){
    if (wd->next[i] != NULL)
      wdict_free(wd->next[i]);
  }
  free(wd->next);
  free(wd);
}

int _get_char_index(char ch){
  /* index depending on the first letter, with ascii it's easy */
  return (int) (ch - 'a');
}

char _get_index_char(int i){
  return (char)i + 'a';
}

/* Inserisce la parola in wd->next se non e' finita
 * Se e' finita setta il flag in wd
*/
int wdict_insert(struct wdict* wd, const char* word, int n_chars){
  int cp;
  if (strlen(word) == 0){
    wd->end++;
    return 0;
  }
  if (wd == NULL){
    printf("wd==NULL!!!!!");
    return -1;
  }

  cp = _get_char_index(word[0]);
  if (cp > n_chars){
      printf("Errore: carattere %c overflow!!\n", cp);
      return -1;
  }
  if (wd->next[cp] == NULL){
    wd->next[cp] = wdict_alloc(word[0], n_chars);
    if (wd->next[cp] == NULL){
      printf("Errore: OOM!!");
      return -1;
    }
  }
  return wdict_insert(wd->next[cp], ++word, n_chars);
}

/* create an optimized dictionary structure from words in a text file */
dict_t* load_dict(const char* fname)
{
  FILE* f;
  char* wbuf;
  size_t nbuf=20;
  dict_t* d;

  f = fopen(fname, "r");
  if (f == NULL){
    perror("Apertura dict");
    return NULL;
  }

  /* create root node */
  d = malloc(sizeof(dict_t));
  d->n_chars = 26;
  d->dict = wdict_alloc(WDICT_ROOT, d->n_chars);
  if (d->dict == NULL){
    perror("Alloc dict");
    return NULL;
  }
  d->dlen = 0;

  /* use a buffer to read and insert a word into the dict */
  wbuf = malloc((nbuf + 1)*sizeof(char));
  while (getline(&wbuf, &nbuf, f) > 0){
    int i;
    for (i=0; i<strlen(wbuf); i++){
      /* truncate word on line end */
      if ((wbuf[i] == '\n') || (wbuf[i] == '\r'))
	wbuf[i] = '\0';
    }
    if (wdict_insert(d->dict, wbuf, d->n_chars) == 0)
      d->dlen++;
  };
  free(wbuf);

  fclose(f);

  return d;
}

void free_dict(dict_t* d){
  wdict_free(d->dict);
  free(d);
}

void _walk_subtree(struct wdict* wd, char* partial, int n_chars, word_f f){
  int i;
  int subtree_pos = strlen(partial);
  char* next_partial = malloc(subtree_pos+2);

  snprintf(next_partial, subtree_pos+2, "%s%c", partial, wd->ch);

  if (wd->end > 0)
    f(next_partial);

  for (i=0; i<n_chars; i++){
    if (wd->next[i] != NULL)
      _walk_subtree(wd->next[i], next_partial, n_chars, f);
  }

  free(next_partial);
}

/* walk a whole dict and call f for every word in the dict */
void walk_dict(dict_t* d, word_f f){
  int i;
  char* tmps = malloc(sizeof(char));

  for (i=0; i<d->n_chars; i++){
    if (d->dict->next[i] != NULL){
      tmps[0] = '\0';
      _walk_subtree(d->dict->next[i], tmps, d->n_chars, f);
    }
  }

  free(tmps);
}

void _print_word(char* word){
  printf("%s\n", word);
}

void print_dict(dict_t *d){
  walk_dict(d, _print_word);
}

int lookup_dict(dict_t* d, const char* word){
  int i, ret, found=0;
  struct wdict* wd=d->dict;
  int wlen=strlen(word);
  int cindex;

  for (i=0; i<wlen; i++){
    cindex = _get_char_index(word[i]);
    if (wd->next[cindex] != NULL){
      wd = wd->next[cindex];
      if ((wd->end > 0) && (i == wlen-1))
	found += 1;
    }
    else
      break;
  }

  if (i == wlen){
    if (found == 1)
      /* full word found */
      ret = 0;
    else
      /* half word found */
      ret =  1;
  } else {
    /* no words possible */
    ret = -1;
  }

  return ret;
}
