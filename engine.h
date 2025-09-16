#ifndef ENGINE_H
#define ENGINE_H

#include <time.h>

#include "game.h"
#include "cthread.h"
#include "tt.h"

typedef struct {
	int moves[MAXGRID];
	int score[MAXGRID];
	int size;
} Move;

// no.
typedef struct {
    int moves[MAXGRID];
    int size;
} Pv;
//
// find move acording to best score for a thread.
Tmoven Findmovethread(State *st, int free, int depth, int id);
// automate thread initiation and return the best move searched.
int Findmovet(State *st, int depth, int threadcount);

int Lsb(int n);

// struct
int Findmovestruct(State *st, int depth);

int nMinimax(State *st, int alpha, int beta, int *nodes, int depth, int ismax);

int ttMinimax(State *st, Ttable *tt, int alpha, int beta, int *nodes, int depth, int ismax);

#endif // ENGINE_H
