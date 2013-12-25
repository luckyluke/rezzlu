
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
  /* solve all? */
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

game_t* game_load(char** raw_game, game_config_t* cfg);
game_t* game_gen(game_config_t* cfg);

word_t* game_put_char(game_status_t* status, cell_t* cell);
word_t* game_stop_word(game_status_t* status);
int game_check_word(game_status_t* status, word_t* word);


#endif /* GAME_H */
