
#ifndef SOLVER_H
#define SOLVER_H

#include "dict.h"

typedef struct {
} game_t;

typedef struct {
} solution_t;

game_t* load_game(const char** raw_game);
solution_t* solve(game_t game, dict_t d);

#endif /* SOLVER_H */
