
#ifndef SOLVER_H
#define SOLVER_H

#include "dict.h"
#include "game.h"

typedef struct {
  word_t** words;
  int nwords;
} solution_t;

solution_t* solution_alloc(void);
void solution_free(solution_t* s);
void solve_game(game_t* game, solution_t*sol);
void print_solution(solution_t* s);

#endif /* SOLVER_H */
