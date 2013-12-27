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
  g = game_alloc(TEST_CFG);
  game_load(TEST_GAME, g);

  fprintf(stderr, "Inizio Soluzione ...\n");
  s = solution_alloc();
  solve_game(g, s);

  print_solution(s);

  solution_free(s);
  game_free(g);
  free_dict(d);

  return 0;
}
