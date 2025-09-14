#ifndef THREAD_H
#define THREAD_H
#include <windows.h>
#include "game.h"

// structure to handle threads.
typedef struct {
	HANDLE *thread;
	int size;
} Thread;

// structure to handle threads.
typedef struct {
	int amount;
	int depth;
	State st;
	char id;
} Threadparam;

// structure to handle return value of Findmovethread -> move and score.
typedef struct {
	int score;
	int move;
} Tmove;

// array to hold all thread best moves and score.
extern Tmove Movelist[2000];

// function to initialize a thread.
void Inthread(Thread *thread);

// function to create threads.
void Createthreads(Thread *thread, State *st, int size, int depth);

#endif // THREAD_H
