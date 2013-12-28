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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"

word_t* word_alloc(char* w, path_t* p){
  word_t* tmp;
  int wsize=sizeof(char)*(strlen(w)+1);

  if ((tmp = malloc(sizeof(word_t))) == NULL){
    perror("alloc word");
    return NULL;
  }

  if ((tmp->word = malloc(wsize)) == NULL){
    free(tmp);
    perror("alloc word string");
    return NULL;
  }
  strncpy(tmp->word, w, wsize);
  if ((tmp->path = path_copy(p)) == NULL){
    free(tmp->word);
    free(tmp);
    perror("alloc word path");
    return NULL;
  }
  tmp->pathlen = path_len(tmp->path);

  return tmp;
}

void word_free(word_t* w){
  if (w == NULL)
    return;
  free(w->word);
  path_free(w->path);
  free(w);
}

game_t* game_alloc(game_config_t* cfg){
  game_t* tmp;
  int i;

  if ((tmp = malloc(sizeof(game_t))) == NULL){
    perror("alloc game");
    return NULL;
  }

  if ((tmp->ch = malloc(sizeof(char*)*cfg->rows)) == NULL){
    perror("alloc game rows");
    game_free(tmp);
    return NULL;
  }  

  for (i=0; i<cfg->rows; i++){
    if ((tmp->ch[i] = malloc(sizeof(char*)*cfg->cols)) == NULL){
      perror("alloc game column");
      if (i>0){
	int j;
	for (j=i-1; j>0; j--)
	  free(tmp->ch[j]);
      }
      game_free(tmp);
      return NULL;
    }
  }

  tmp->cfg = cfg;
  return tmp;
}

void game_free(game_t* g){
  if (g->ch != NULL){
    int i;
    for (i=0; i<g->cfg->rows; i++)
      free(g->ch[i]);
    free(g->ch);
  }
  free(g);
}

void game_load(char** raw_game, game_t* g){
  int i, j;

  if (g != NULL)
    for (i=0; i<g->cfg->rows; i++)
      for (j=0; j<g->cfg->cols; j++)
	g->ch[i][j] = raw_game[i][j];
}

void game_gen(game_t* g)
{
  /* TODO */
}

word_t* game_put_char(game_status_t* status, cell_t* cell){
  /* TODO */
  return &(status->tmpw);
}

word_t* game_stop_word(game_status_t* status){
  /* TODO */
  return NULL;
}

int game_check_word(game_status_t* status, word_t* word){
  /* TODO */
  return 0;
}
