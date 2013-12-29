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

#ifndef GAME_H
#define GAME_H

#include "dict.h"
#include "path.h"

typedef struct {
  char* word;
  path_t* path;
  unsigned short pathlen;
} word_t;

typedef struct game_config_s {
  short int rows;
  short int cols;
  dict_t* dict;
  int solve_all;
} game_config_t;

typedef struct {
  char** ch;
  cell_t bonus_dw;
  cell_t bonus_tw;
  cell_t bonus_dc;
  cell_t bonus_tc;
  game_config_t* cfg;
} game_t;

typedef struct game_status_s {
  word_t tmpw;
  int score;
  game_t* g;
} game_status_t;

word_t* word_alloc(char* w, path_t* p);
void word_free(word_t* w);
game_t* game_alloc(game_config_t* cfg);
void game_free(game_t* g);
void game_load(char** raw_game, game_t* g);

void game_gen(game_t* g);
word_t* game_put_char(game_status_t* status, cell_t* cell);
word_t* game_stop_word(game_status_t* status);
int game_check_word(game_status_t* status, word_t* word);


#endif /* GAME_H */
