#include <stdio.h>

#include "solver.h"
#include "test.h"

int main(int argc, const char** argv)
{
  dict_t* d;
  fprintf(stderr, "Carico dizionario e schema ...\n");
  d = load_dict("../dicts/ita.dict");
  /* g = TEST; */
  /* game = load_game(g); */
  /* fprintf(2, "Inizio Soluzione ..."); */
  /* sol = solve(game, d); */
  return 0;
}
