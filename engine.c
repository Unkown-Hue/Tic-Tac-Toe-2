#include "engine.h"
#include "eval.h"
#include "cthread.h"
#include <minwindef.h>
#include <winbase.h>
#define GDBUG


void Extract(Move *move, int num){
    move->size = 0;
	int i = 0;
	while (num){
		move->moves[i] = Poplsb(&num);
		i++;
        move->size++;
	}
}

void SortX(int x, int o, Move *move){
    int i;
    int j;
    for (i = 0; i < move->size; i++){
        move->score[i] = Eval(x | (1 << move->moves[i]), o);
        }
    for (i = 0; i < move->size; i++){
        for (j = i + 1; j < move->size; j++){
            if (move->score[j] > move->score[i]){
                int temp = move->score[i];
                move->score[i] = move->score[j];
                move->score[j] = temp;
                temp = move->moves[i];
                move->moves[i] = move->moves[j];
                move->moves[j] = temp;
            }
        }
    }
}

void SortO(int x, int o, Move *move){
    int i;
    int j;
    for (i = 0; i < move->size; i++){
        move->score[i] = Eval(o | (1 << move->moves[i]), x);
        }
    for (i = 0; i < move->size; i++){
        for (j = i + 1; j < move->size; j++){
            if (move->score[j] > move->score[i]){
                int temp = move->score[i];
                move->score[i] = move->score[j];
                move->score[j] = temp;
                temp = move->moves[i];
                move->moves[i] = move->moves[j];
                move->moves[j] = temp;
            }
        }
    }
}

Tmove Findmovethread(State *st, int free, int depth, int id){
    Tmove tmove;
    Move move;
    int bestmove = -1;
    int score;
    int alpha = -999999;
    int bit;
    int beta = 999999;
    int nodes = 0;
    int i = 0;
    Extract(&move, free);
    if (st->move == X){
        SortX(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
        	Play(st, bit);
        	score = nMinimax(st, alpha, beta, &nodes, depth - 1, 0);
        	Undo(st);
        	if (score > alpha){
        		bestmove = bit;
        		alpha = score;
        	}
            i++;
        }
    }
    else {
        SortO(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
        	Play(st, bit);
        	score = nMinimax(st, alpha, beta, &nodes, depth - 1, 1);
        	Undo(st);
        	if (score < beta){
        		bestmove = bit;
        		beta = score;
        	}
            i++;
        }
    }
    //printf("nodes searched: %d from id: %d\n", nodes, id);
    tmove.move = bestmove;
    tmove.score = st->move == X ? alpha : beta;
    return tmove;
}

int Findmovet(State* st, int depth, int threadcount){
    Tmove move = {st->move == X ? -999999 : 999999, 0};
    Thread thread;
    Inthread(&thread);
    Createthreads(&thread, st, threadcount, depth);
    for (int i = 0; i < threadcount; i++){
        if (st->move == X){
            if (Movelist[i].score > move.score){
                move = Movelist[i];
            }
        }
        else{
            if (Movelist[i].score < move.score){
                move = Movelist[i];
            }
        }
    }
    return move.move;
}

int Findmovestruct(State *st, int depth){
    Move move;
    int bestmove = -1;
    int score;
    int free = All(st) ^ MAXBIT;
    int alpha = -999999;
    int bit;
    int beta = 999999;
    int nodes = 0;
    int i = 0;
    Extract(&move, free);
    if (st->move == X){
        SortX(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
        	Play(st, bit);
        	score = nMinimax(st, alpha, beta, &nodes, depth - 1, 0);
        	Undo(st);
        	if (score > alpha){
        		bestmove = bit;
        		alpha = score;
        	}
            i++;
        }
    }
    else {
        SortO(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
        	Play(st, bit);
        	score = nMinimax(st, alpha, beta, &nodes, depth - 1, 1);
        	Undo(st);
        	if (score < beta){
        		bestmove = bit;
        		beta = score;
        	}
            i++;
        }
    }
    printf("nodes searched: %d\n", nodes);
    return bestmove;
}

int nMinimax(State *st, int alpha, int beta, int *nodes, int depth, int ismax){
	*nodes = *nodes + 1;
    if (Win(st, X)) {
        return 1000 + depth * 20;
    }
    if (Win(st, O)) {
        return -1000 - depth * 20;
    }
    if (Full(st)){
		return 0;
	}
	if (depth <= 0){
		/*
		printf("Minimax\n");
		Printboard(st);
		printbin(st->side[side]);
		printbin(st->side[side ^ O]);
		printf("Score: %d\n", Eval(&st->side[side], &st->side[side ^ O]));
		*/
		return Eval(st->side[X], st->side[O]);
	}
	// do maximizing and call mimimin
    Move move;
	int free = All(st) ^ MAXBIT;
	int bit;
    int score;
    int i = 0;
    SIDE side = st->move;
    Extract(&move, free);
    if (ismax){
        SortX(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
   	        Play(st, bit);
	        score = nMinimax(st, alpha, beta, nodes, depth - 1, 0);
	        Undo(st);
	        if (score > alpha){
	    	    alpha = score;
	        }
	        if (beta <= alpha) break;
            i++;
        }
	    return alpha;
    }
    else {
        SortO(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
   	        Play(st, bit);
	    	score = nMinimax(st, alpha, beta ,nodes, depth - 1, 1);
	    	Undo(st);
	    	if (score < beta){
	    		beta = score;
	    	}
	    	if (beta <= alpha) break;
            i++;
       }
	    return beta;
    }
}

