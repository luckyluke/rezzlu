#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "solver.h"

path_t* path_alloc(int x, int y){
  path_t* p;
  if ((p=malloc(sizeof(path_t))) == NULL){
    perror("alloc path");
    return NULL;
  }
  p->cell.x = x;
  p->cell.y = y;
  p->next = NULL;
  return p;
}

void path_free(path_t* p){
  path_t* tmpp=p;
  while (p != NULL){
    tmpp = p->next;
    free(p);
    p = tmpp;
  }
}

void path_append(path_t* p, int x, int y){
  if (p == NULL)
    return;

  while (p->next != NULL)
    p = p->next;

  p->next = path_alloc(x, y);
}

void path_chop(path_t* p){
  if (p == NULL)
    return;

  while ((p->next != NULL) && (p->next->next != NULL))
    p = p->next;
  if (p->next){
    path_free(p->next);
    p->next = NULL;
  }
}

int path_equals(path_t* p1, path_t* p2){
  if ((p1==NULL) && (p2==NULL))
    /* both empty */
    return 1;

  if ((p1==NULL) || (p2==NULL))
    /* only one empty */
    return 0;

  while ((p1->next != NULL) && (p2->next != NULL)){
    if ((p1->cell.x != p2->cell.x) || (p1->cell.y != p2->cell.y))
      /* element at same position is different */
      return 0;
    p1 = p1->next;
    p2 = p2->next;
  }

  if ((p1->next != NULL) || (p2->next != NULL))
    /* paths have different length */
    return 0;
  else
    return 1;
}

/* path p containg cell (x, y) */
int path_contains(path_t* p, int x, int y){
  while (p != NULL){
    if ((p->cell.x == x) && (p->cell.y == y))
      return 1;
    p = p->next;
  }
  return 0;
}

void word_free(word_t* w);
void solution_free(solution_t* s);

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

solution_t* solve_game_char(game_t* g, dict_t* d, char* curw,
			    int row, int col, path_t* p, solution_t* sol){
  int r, c;
  int newlen = strlen(curw)+2;

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

  /* { */
  /*   path* pp=p; */
  /*   while (pp!=NULL){ */
  /*     printf("x %d y %d - ", pp->x, pp->y); */
  /*     pp = pp->next; */
  /*   } */
  /*   printf("\n"); */
  /* } */

	sol->nwords++;
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

solution_t* solve_game(game_t* game, dict_t* d){
  solution_t* sol;
  int i, j;

  sol = solution_alloc();
  for (i=0; i<game->cfg->rows; i++)
    for (j=0; j<game->cfg->cols; j++){
      path_t* start_path;
      char* starts;

      start_path=path_alloc(i, j);
      starts=malloc(2*sizeof(char));

      snprintf(starts, 2, "%c", game->ch[i][j]);
      solve_game_char(game, d, starts, i, j, start_path, sol);

      free(start_path);
      free(starts);
    }

  return sol;
}

void print_solution(solution_t* s){
  printf("sol %d\n", s->nwords);
}
