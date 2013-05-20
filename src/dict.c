#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "dict.h"


/* struct wlist* wlist_alloc(char* word){ */
/*   struct wlist* wl; */
/*   wl = malloc(sizeof(struct wlist)); */
/*   if (wl){ */
/*     size_t wlen = strlen(word); */
/*     wl->word = malloc((wlen+1)*sizeof(char)); */
/*     strcpy(wl->word, word); */
/*     wl->next = NULL; */
/*   } */
/*   return wl; */
/* } */

/* struct wlist* get_next(struct wlist* wl){ */
/*   while (wl->next != NULL) */
/*     wl = wl->next; */
/*   return wl; */
/* } */

/* struct wlist* wlist_append(struct wlist* wl, char* word){ */
/*   struct wlist *newl, *oldl; */
/*   if (! wl){ */
/*     wl = wlist_alloc(word); */
/*     return wl; */
/*   } */
/*   newl = wlist_alloc(word); */
/*   if (! newl){ */
/*     perror("wlist append"); */
/*     return NULL; */
/*   } */
/*   oldl = wl; */
/*   wl = get_next(wl); */
/*   wl->next = newl; */
/*   return oldl; */
/* } */


/* dict_t* load_dict(const char* fname) */
/* { */
/*   FILE* f; */
/*   char* wbuf; */
/*   size_t nbuf=20; */
/*   dict_t* d; */
/*   struct wlist* wl; */

/*   f = fopen(fname, "r"); */
/*   if (f == NULL){ */
/*     perror("Apertura dict"); */
/*     return NULL; */
/*   } */

/*   d = malloc(sizeof(dict_t)); */
/*   d->dlist = NULL; */
/*   d->dlen = 0; */

/*   wbuf = malloc((nbuf + 1)*sizeof(char)); */
/*   while (getline(&wbuf, &nbuf, f) > 0){ */
/*     printf("Append\n"); */
/*     d->dlist = wlist_append(d->dlist, wbuf); */
/*     d->dlen++; */
/*     printf("%d\n", d->dlen); */
/*   }; */
/*   free(wbuf); */

/*   printf("Caricate %d parole\n", d->dlen); */
/*   wl = d->dlist; */
/*   while (wl != NULL){ */
/*     printf("%s\n", wl->word); */
/*     wl = wl->next; */
/*   } */

/*   fclose(f); */

/*   return d; */
/* } */




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
  return NULL;
}

void wdict_free(struct wdict* wd){
}

int _get_char_index(char ch){
  /* index depending on the first letter, with ascii it's easy */
  return (int) (ch - 'a');
}

struct wdict* wdict_insert(struct wdict* wd, const char* word, int n_chars){
  int cp;
  struct wdict* tmpd;
  if (strlen(word) == 0)
    return NULL;
  //if (wd == NULL)
  // return NULL;

  cp = _get_char_index(word[0]);
  if (cp > n_chars){
      printf("Errore: carattere %c overflow!!\n", cp);
      return NULL;
    }
  if (wd->next[cp] == NULL){
    wd->next[cp] = wdict_alloc(cp, n_chars);
    if (wd->next[cp] == NULL){
      printf("Errore: OOM!!");
      return NULL;
    }
  }
  return wdict_insert(wd->next[cp], ++word, n_chars);
  tmpd = wdict_insert(wd->next[cp], ++word, n_chars);
  printf("%s %p\n", word,  tmpd);
  if (tmpd != NULL)
    return wd;
  else
    return NULL;
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

  d = malloc(sizeof(dict_t));
  d->n_chars = 26;
  d->dict = wdict_alloc((char)0, d->n_chars);
  if (d->dict == NULL){
    perror("Alloc dict");
    return NULL;
  }
  d->dlen = 0;

  wbuf = malloc((nbuf + 1)*sizeof(char));
  while (getline(&wbuf, &nbuf, f) > 0){
    wdict_insert(d->dict, wbuf, d->n_chars);
    /* if (wdict_insert(d->dict, wbuf, d->n_chars) == NULL){ */
    /*   wdict_free(d->dict); */
    /*   printf("OOM!!!\n"); */
    /*   break; */
    /* } */
    d->dlen++;
  };
  free(wbuf);

  printf("Caricate %d parole\n", d->dlen);
  
  /* struct wlist* wl=d->dlist; */
  /* printf("%x\n", wl); */
  /* while (wl != NULL){ */
  /*   printf("%s\n", wl->word); */
  /*   wl = wl->next; */
  /* } */

  fclose(f);

  return d;
}

void free_dict(dict_t* d){
}

int lookup_dict(dict_t* d, const char word){
  return 0;
}

/* dict_t* load_dict(const char* fname) */
/* { */
/*   FILE* f; */
/*   char* wbuf; */
/*   size_t nbuf=20; */
/*   dict_t* d; */
/*   int di=0; */

/*   f = fopen(fname, "r"); */
/*   if (f == NULL){ */
/*     perror("Apertura dict"); */
/*     return NULL; */
/*   } */
/*   mtrace(); */

/*   d = malloc(sizeof(dict_t)); */
/*   d->dlist = (char**)malloc(__DEF_DICT_LEN*sizeof(char*)); */
/*   d->dlen = __DEF_DICT_LEN; */
/*   wbuf = malloc((nbuf + 1)*sizeof(char)); */

/*   while (getline(&wbuf, &nbuf, f) > 0){ */
/*     char* wtmp; */
/*     int len; */
/*     struct mallinfo minfo; */

/*     minfo = mallinfo(); */
/*     printf("%d %d\n", minfo.arena, minfo.hblkhd); */
/*     len = strlen(wbuf); */
/*     //wtmp = malloc((len + 1)*sizeof(char)); */
/*     //strcpy(wtmp, wbuf); */
/*     //d->dlist[di] = wtmp; */
/*     printf("di %d len %d dlen %d\n", di, len, d->dlen); */
/*     d->dlist[di] = malloc((len + 1)*sizeof(char)); */
/*     if (d->dlist[di] == NULL){ */
/*       perror("malloc"); */
/*       break; */
/*     } */
/*     strcpy(d->dlist[di], wbuf); */
/*     if (++di > d->dlen){ */
/*       int new_dlen; */
/*       new_dlen = d->dlen * 2; */
/*       if (!(realloc(d->dlist, new_dlen*sizeof(char*)))){ */
/* 	perror("Caricamento dict"); */
/* 	break; */
/*       } */
/*       printf("realloc %d\n", new_dlen); */
/*       d->dlen = new_dlen; */
/*     } */
/*   }; */

/*   fclose(f); */

/*   return d; */
/* } */
