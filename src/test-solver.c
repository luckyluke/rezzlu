#include <stdio.h>

#include "solver.h"
#include "test.h"

int main(int argc, const char** argv)
{
  dict_t* d;
  game_t* g;
  solution_t* s;
  fprintf(stderr, "Carico dizionario e schema ...\n");
  d = load_dict(TEST_DICT);
  TEST_CFG->dict = d;
  g = game_load(TEST_GAME, TEST_CFG);
  fprintf(stderr, "Inizio Soluzione ...\n");
  s = solve_game(g, d);
  print_solution(s);
  return 0;
}
