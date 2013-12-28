/*
 * Copyright (C) 2013 Luca Dariz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
  d = load_dict(TEST_DICT);
  words_walk=0;
  walk_dict(d, check_findable);
  printf("Loaded: %d Walked: %d\n", d->dlen, words_walk);
  free_dict(d);

  return 0;
}
