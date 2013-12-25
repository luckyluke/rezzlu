
#include <stdio.h>
#include <stdlib.h>

#include "path.h"

path_t* path_alloc(int x, int y){
  path_t* p;
  if ((p=malloc(sizeof(path_t))) == NULL){
    perror("alloc path");
    return NULL;
  }
  p->cell.x = x;
  p->cell.y = y;
  p->next = NULL;
  return p;
}

void path_free(path_t* p){
  path_t* tmpp=p;
  while (p != NULL){
    tmpp = p->next;
    free(p);
    p = tmpp;
  }
}

void path_append(path_t* p, int x, int y){
  if (p == NULL)
    return;

  while (p->next != NULL)
    p = p->next;

  p->next = path_alloc(x, y);
  if (p->next == NULL)
    perror("append path");
}

void path_chop(path_t* p){
  if ((p == NULL) || (p->next == NULL))
    return;

  while (p->next->next != NULL)
    p = p->next;

  path_free(p->next);
  p->next = NULL;
}

int path_equals(path_t* p1, path_t* p2){
  if ((p1==NULL) && (p2==NULL))
    /* both empty */
    return 1;

  if ((p1==NULL) || (p2==NULL))
    /* only one empty */
    return 0;

  while ((p1->next != NULL) && (p2->next != NULL)){
    if ((p1->cell.x != p2->cell.x) || (p1->cell.y != p2->cell.y))
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
int path_contains(path_t* p, int x, int y){
  while (p != NULL){
    if ((p->cell.x == x) && (p->cell.y == y))
      return 1;
    p = p->next;
  }
  return 0;
}
