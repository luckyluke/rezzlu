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

#ifndef TEST_H
#define TEST_H

#include "game.h"

#define TEST_ROWS 4
#define TEST_COLS 4

game_config_t _TEST_CFG = {TEST_ROWS,
			   TEST_COLS,
			   NULL};

#define TEST_CFG (&(_TEST_CFG))

char* TEST_GAME[TEST_ROWS] = {"gehc",
			      "rsgy",
			      "cebu",	
			      "dyga"};

/* char* TEST_GAME[TEST_ROWS] = {"rsgyauharj", */
/* 			      "rsgybanaro", */
/* 			      "cebucnaonn", */
/* 			      "cebucnaonn", */
/* 			      "dygadioanr", */
/* 			      "cebucnaonn", */
/* 			      "cebucnaonn", */
/* 			      "dygadioanr", */
/* 			      "dygadioanr", */
/* 			      "ihybaachor"}; */


/* char* TEST_GAME[TEST_ROWS] = {"rsgy", */
/* 			      "rsgy", */
/* 			      "cebu", */
/* 			      "dyga"}; */

#define TEST_DICT "../dicts/ita.dict"

#endif /* TEST_H */
