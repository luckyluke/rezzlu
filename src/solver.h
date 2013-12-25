
#ifndef SOLVER_H
#define SOLVER_H

#include "dict.h"
#include "game.h"

typedef struct {
  game_t game;
  word_t** words;
  int nwords;
} solution_t;

solution_t* solve_game(game_t* game, dict_t* d);
void print_solution(solution_t* s);

#endif /* SOLVER_H */
