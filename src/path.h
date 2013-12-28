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
