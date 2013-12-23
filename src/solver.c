#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"
#include "solver.h"


typedef struct _path {
  struct _path* next;
  int x;
  int y;
} path;

path* path_alloc(int x, int y){
  path* p;
  if ((p=malloc(sizeof(path))) == NULL){
    perror("alloc path");
    return NULL;
  }
  p->x = x;
  p->y = y;
  p->next = NULL;
  return p;
}

void path_free(path* p){
  path* tmpp=p;
  while (p != NULL){
    tmpp = p->next;
    free(p);
    p = tmpp;
  }
}

void path_append(path* p, int x, int y){
  if (p == NULL)
    return;

  p->next = path_alloc(x, y);
}

void path_chop(path* p){
  if (p == NULL)
    return;

  while ((p->next != NULL) && (p->next->next != NULL))
    p = p->next;
  if (p->next){
    free(p->next);
    p->next = NULL;
  }
}

int path_equals(path* p1, path* p2){
  if ((p1==NULL) && (p2==NULL))
    /* both empty */
    return 1;

  if ((p1==NULL) || (p2==NULL))
    /* only one empty */
    return 0;

  while ((p1->next != NULL) && (p2->next != NULL)){
    if ((p1->x != p2->x) || (p1->y != p2->y))
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
int path_contains(path* p, int x, int y){
  while (p != NULL){
    if ((p->x == x) && (p->y == y))
      return 1;
    p = p->next;
  }
  return 0;
}

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
      if (i>0){
	int j;
	for (j=i-1; j>0; j--)
	  free(tmp->ch[j]);
      }
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
  if (g != NULL)
    for (i=0; i<rows; i++)
      for (j=0; j<columns; j++)
	g->ch[i][j] = raw_game[i][j];
  return g;
}

solution_t* solve_game_char(game_t* g, dict_t* d, char* curw,
			    int row, int col, path* p, solution_t* sol){
  int r, c;
  int newlen = strlen(curw)+2;

  for (r=row-1; r<=row+1; r++)
    for (c=col-1; c<=col+1; c++){
      char* neww;
      solution_t* news;
      int found;

      //printf("r %d c %d\n", r, c);

      /* check overflow */
      if ((r < 0) || (c < 0) || (r >= g->rows) || (c >=g->columns))
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
      /* FIXME: build newpath, usa linked list come path*/
      path_append(p, r, c);
      found = lookup_dict(d, neww);
      if (found == -1)
	continue;
      else if (found == 0){
	/* TODO: append word to solution*/
	do {} while(0);
	printf("neww %s\n", neww);

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
  for (i=0; i<game->rows; i++)
    for (j=0; j<game->columns; j++){
      path* start_path;
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
