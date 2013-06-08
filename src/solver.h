
#ifndef SOLVER_H
#define SOLVER_H

#include "dict.h"

typedef struct {
  char** ch;
  short int rows;
  short int columns;
} game_t;

typedef struct {
  char* word;
  short int** path;
} word_t;

typedef struct {
  game_t game;
  word_t** words;
  int nwords;
} solution_t;

game_t* load_game(char** raw_game, int rows, int columns);
solution_t* solve_game(game_t* game, dict_t* d);
void print_solution(solution_t* s);

#endif /* SOLVER_H */
