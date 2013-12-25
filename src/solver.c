#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"
#include "dict.h"
#include "game.h"
#include "solver.h"

solution_t* solution_alloc(){
  solution_t* tmp;

  if ((tmp = malloc(sizeof(solution_t))) == NULL){
    perror("alloc solution");
    return NULL;
  }

  tmp->words = NULL;
  tmp->nwords = 0;
  return tmp;
}

void solution_append(solution_t* sol, char* neww, path_t* newp){
  sol->nwords++;
}

void solution_free(solution_t* s){
  free(s);
}

solution_t* solve_game_char(game_t* g, dict_t* d, char* curw,
			    int row, int col, path_t* p, solution_t* sol){
  int r, c;
  int newlen = strlen(curw)+2; /* include string terminator */

  for (r=row-1; r<=row+1; r++)
    for (c=col-1; c<=col+1; c++){
      char* neww;
      int found;

      /*printf("r %d c %d\n", r, c);*/

      /* check overflow */
      if ((r < 0) || (c < 0) || (r >= g->cfg->rows) || (c >=g->cfg->cols))
	continue;
      /* starting point */
      if ((r == row) && (c == col))
	continue;
      /* already in path */
      if (path_contains(p, r, c))
	continue;
      /* ... */

      if ((neww = malloc(newlen)) == NULL){
	perror("malloc solver");
	return NULL;
      }

      snprintf(neww, newlen, "%s%c", curw, g->ch[r][c]);
      path_append(p, r, c);
      found = lookup_dict(d, neww);
      if (found == -1)
	continue;
      else if (found == 0){
	/* TODO: append word to solution*/
	do {} while(0);
	printf("%d %s\n", found,  neww);
	solution_append(sol, neww, p);
      }

      if (solve_game_char(g, d, neww, r, c, p, sol) == NULL){
	path_chop(p);
	free(neww);
	return NULL;
      }

      path_chop(p);
      free(neww);
    }
  
  return sol;
}

solution_t* solve_game(game_t* game, solution_t* sol){
  int i, j;

  for (i=0; i<game->cfg->rows; i++)
    for (j=0; j<game->cfg->cols; j++){
      path_t* start_path;
      char* starts;

      start_path=path_alloc(i, j);
      starts=malloc(2*sizeof(char));

      snprintf(starts, 2, "%c", game->ch[i][j]);
      solve_game_char(game, game->cfg->dict, starts, i, j, start_path, sol);

      free(start_path);
      free(starts);
    }

  return sol;
}

void print_solution(solution_t* s){
  printf("sol %d\n", s->nwords);
}
