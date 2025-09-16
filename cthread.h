#ifndef THREAD_H
#define THREAD_H

#include "game.h"

#ifdef _Win32
#include <windows.h>
#include <tlhelp32.h>

// structure to handle threads.
typedef struct {
	HANDLE *thread;
	int size;
} Thread;
// function to initialize a thread.
void Inthread(Thread *thread);
// function to create threads.
void Createthreads(Thread *thread, State *st, int size, int depth);

#elif __linux__
#include <pthread.h>

typedef struct {
	pthread_t *thread;
	int size;
} Thread;
// function to initialize a thread.
void Inthread(Thread *thread);
// function to create threads.
void Createthreads(Thread *thread, State *st, int size, int depth);

#endif

#include "game.h"



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

// structure to handle return value of Findmovethread -> moves score and node count.
typedef struct {
	int score;
	int move;
    unsigned long long nodes;
} Tmoven;

// array to hold all thread best moves and score.
extern Tmoven Movelist[2000];
// function to count threads
int Getthreadcount();

// function to count threads created 
int Getproccescount();

#endif // THREAD_H
