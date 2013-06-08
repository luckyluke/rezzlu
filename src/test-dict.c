#include <stdio.h>

#include "dict.h"
#include "test.h"

dict_t* d;
int words_walk;

void check_findable(char* word){
  int fret;

  /* printf("Verifica per %s\n", word); */
  fret = lookup_dict(d, word);
  if (fret < 0)
    printf("lookup %s failed!", word);
  else{
    if (fret > 0)
      printf("lookup %s gave too much words!", word);
    else
      words_walk++;
  }
}

int main(int argc, const char** argv)
{
  d = load_dict("../dicts/ita.dict");
  words_walk=0;
  walk_dict(d, check_findable);
  printf("Loaded: %d Walked: %d\n", d->dlen, words_walk);
  free_dict(d);

  return 0;
}
