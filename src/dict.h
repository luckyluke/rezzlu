/*
 * Copyright (C) 2013 Luca Dariz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

/* a symbol of the alphabet used, with the relative frequency */
typedef struct character_s {
  char c;
  float freq;
} character_t;

typedef struct {
  /* number of chars in the alphabet */
  int n_chars;
  character_t* chars;
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
