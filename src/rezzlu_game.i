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

%module rezzlu_game
%{
#include "solver.h"
#include "game.h"
#include "dict.h"
%}

%include "solver.h"
%include "game.h"
%include "dict.h"

/* getter/setter for char tables like game_t.ch */
%inline %{
void char_set(char** ch, int i, int j, char c) {
  ch[i][j] = c;
}
char char_get(char** ch, int i, int j) {
  return ch[i][j];
}
%}

