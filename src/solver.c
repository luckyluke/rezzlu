#include <stdio.h>
#include <stdlib.h>

#include "dict.h"
#include "solver.h"

void game_free(game_t* g);
void word_free(word_t* w);
void solution_free(solution_t* s);

game_t* game_alloc(int rows, int columns){
  game_t* tmp;
  int i;

  if ((tmp = malloc(sizeof(game_t))) == NULL){
    perror("alloc game");
    return NULL;
  }

  if ((tmp->ch = malloc(sizeof(char*)*rows)) == NULL){
    perror("alloc game rows");
    game_free(tmp);
    return NULL;
  }

  for (i=0; i<rows; i++){
    if ((tmp->ch[i] = malloc(sizeof(char*)*columns)) == NULL){
      perror("alloc game column");
      game_free(tmp);
      return NULL;
    }
  }
  
  tmp->rows = rows;
  tmp->columns = columns;
  return tmp;
}

void game_free(game_t* g){
  if (g->ch != NULL){
    int i;
    for (i=0; i<g->rows; i++){
      if (g->ch[i] != NULL)
	free(g->ch[i]);
    }
    free(g->ch);
  }
  free(g);
}

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

solution_t* solution_alloc(){
  solution_t* tmp;

  if ((tmp = malloc(sizeof(solution_t))) == NULL){
    perror("alloc solution");
    return NULL;
  }

  return tmp;
}

void solution_extend(solution_t* base, solution_t* new){
}

void solution_free(solution_t* s){
  free(s);
}

game_t* load_game(char** raw_game, int rows, int columns){
  int i, j;
  game_t* g;

  g = game_alloc(rows, columns);
  for (i=0; i<rows; i++)
    for (j=0; j<columns; j++)
      g->ch[i][j] = raw_game[i][j];
  return g;
}

solution_t* solve_game_char(game_t* g, dict_t* d, char* tmpw,
			    int row, int col, int** path){
  int r, c;
  int newlen = strlen(tmpw)+2;
  solution_t* s;

  s = solution_alloc();
  for (r=row-1; r<row+1; r++)
    for (c=col-1; c<col+1; c++){
      char* neww;
      int** newpath;
      solution_t* news;
      int found;

      /* check overflow */
      if ((r < 0) || (c < 0) || (r >= g->rows) || (c >=g->columns))
	continue;
      /* starting point */
      if ((r == row) && (c == col))
	continue;
      /* already in path */
      /* ... */
      neww = malloc(newlen);
      snprintf(neww, newlen, "%s%c", tmpw, g->ch[r][c]);
      found = lookup_dict(d, neww);
      if (found == -1)
	continue;
      /* build newpath */
      else if (found == 0)
	//append
	do {} while(0);

      news = solve_game_char(g, d, neww, r, c, newpath);
      solution_extend(s, news);	
    }
  
  return s;
}

solution_t* solve_game(game_t* game, dict_t* d){
  solution_t* sol;
  int i, j;

  sol = solution_alloc();
  for (i=0; i<game->rows; i++)
    for (j=0; j<game->columns; j++){
      int start_path[1][2];
      char* starts = malloc(2*sizeof(char));
      solution_t* tmpsol;

      start_path[0][0] = i;
      start_path[0][1] = j;
      snprintf(starts, 2, "%c", game->ch[i][j]);

      tmpsol = solve_game_char(game, d, starts, i, j, start_path);
      solution_extend(sol, tmpsol);
      solution_free(tmpsol);
    }

  return sol;
}

void print_solution(solution_t* s){
}
