// game.h
#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>

//Global array of all win patterns
extern int Warray[28];

// macro to define the max amount of squares in a 5x5.
#define MAXGRID 25
#define MAXWIN 28
#define MAXBIT 0b1111111111111111111111111

// macro to set defualt on a state.

// made to be able to use as a boolean. and allocating unsigned char for space.
typedef enum SIDE : unsigned char { X = 0, O = 1 } SIDE;

// struct for keeping the state of a position.
typedef struct {
	// both X and O position.
	int side[2];
	// which sides turn.
	enum SIDE move;
	// array of all moves played.
	int Marray[MAXGRID];
	// move count for the array.
	int mcount;
} State;

// structure for debuging purposes.
typedef struct {
	int *score;
	int *move;
	int nodes;
	int size;
} Moves;


void SetState(State *st);

int All(const State *st);

int Full(const State *st);

// function to play a move from int and make sure its legal.
void Play(State *st, const int m);

// function to play a move from int and make sure its legal with return value.
int Playr(State *st, const int m);

// function to detect win for a side.
int Win(const State *st, enum SIDE side);

// function to detect win for all sides.
int Winall(const State *st);

// function to pop the least significant bit.
int Poplsb(int *n);

// undos a move
void Undo(State *st);

// counts all 1s in a int.
#define Count1s(n) (__builtin_popcountll(n))

// copy for using in threads.
void Copy(State *cst, const State *st);

// init array
void Initw();

// print position.
void Printboard(State *st);

// print with numbers.
void PrintboardNum(State *st);

// sort moves

#endif // GAME_H
