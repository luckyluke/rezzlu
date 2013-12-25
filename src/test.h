
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
