#ifndef EVAL_H
#define EVAL_H

#include "game.h"

// function to return a score from an int.
int Eval(int x, int o);

// function to return a score from an int faster than standard eval.
int Smalleval(int x, int o);

// function to return a score from an int with more advanced heuristics.
int Bigeval(int x, int o, SIDE side);

#endif //EVAL_H
