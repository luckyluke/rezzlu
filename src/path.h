
#ifndef PATH_H
#define PATH_H

typedef struct cell_s {
  short int x;
  short int y;
} cell_t;

typedef struct path_s {
  struct path_s* next;
  cell_t cell;
} path_t;

path_t* path_alloc(int x, int y);
void path_append(path_t* p, int x, int y);
void path_free(path_t* p);
void path_chop(path_t* p);
int path_equals(path_t* p1, path_t* p2);
int path_contains(path_t* p, int x, int y);
path_t* path_copy(path_t* src);
int path_len(path_t* p);
void path_print(path_t* p);

#endif /* PATH_H */
