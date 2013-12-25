
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

game_t* game_alloc(game_config_t* cfg);
void game_free(game_t* g);
void word_free(word_t* w);

word_t* word_alloc(){
  word_t* tmp;

  if ((tmp = malloc(sizeof(word_t))) == NULL){
    perror("alloc word");
    return NULL;
  }

  return tmp;
}

void word_free(word_t* w){
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
    free(g->ch);
  }
  free(g);
}

game_t* game_load(char** raw_game, game_config_t* cfg){
  int i, j;
  game_t* g;

  g = game_alloc(cfg);
  if (g != NULL)
    for (i=0; i<cfg->rows; i++)
      for (j=0; j<cfg->cols; j++)
	g->ch[i][j] = raw_game[i][j];
  return g;
}

game_t* game_gen(game_config_t* cfg)
{
  game_t* g;
  g = game_alloc(cfg);
  /* TODO */
  return g;
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
