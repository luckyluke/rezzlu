#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "dict.h"

struct wdict* wdict_alloc(char ch, int n_chars){
  struct wdict* tmp;
  tmp = malloc(sizeof(struct wdict));
  tmp->ch = ch;
  if (tmp){
    tmp->next = malloc(sizeof(struct wdict*)*n_chars);
    if (tmp->next)
      return tmp;
    free(tmp);
  }
  printf("z\n");
  return NULL;
}

void wdict_free(struct wdict* wd){
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

  printf("Caricate %d parole\n", d->dlen);

  fclose(f);

  return d;
}

void free_dict(dict_t* d){
}

void _print_subtree(struct wdict* wd, char* partial, int n_chars){
  int i;
  int subtree_pos=strlen(partial);
  if (wd->end > 0)
    printf("%s%c\n", partial, wd->ch);

  for (i=0; i<n_chars; i++){
    if (wd->next[i] != NULL){
      partial = realloc(partial, subtree_pos+2);
      partial[subtree_pos] = wd->ch;
      partial[subtree_pos+1] = '\0';
      _print_subtree(wd->next[i], partial, n_chars);
    }
  }
}

void print_dict(dict_t* d){
  int i;
  char* tmps = malloc(sizeof(char));

  for (i=0; i<d->n_chars; i++){
    if (d->dict->next[i] != NULL){
      tmps[0] = '\0';
      _print_subtree(d->dict->next[i], tmps, d->n_chars);
    }
  }

  free(tmps);
}

int lookup_dict(dict_t* d, const char* word){
  int i, ret;
  struct wdict* wd=d->dict;
  int wlen=strlen(word);
  int cindex;
  printf("%d\n", wlen);

  for (i=0; i<wlen; i++){
    cindex = _get_index_char(word[0]);
    if (wd->next[cindex] != NULL){
      if (wd->end > 0){
	ret = 0;
	break;
      }
      wd = wd->next[cindex];
    }
    else if (i == wlen){
      /* dict continua dopo la parola ma la parola non e' intera */
      ret = 1;
      break;
    }
    else{
      /* dict finito prima della parola, non ci puo essere */
      ret = -1;
      break;
    }
  }
  return ret;
}
