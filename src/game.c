
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

word_t* word_alloc(void);
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
