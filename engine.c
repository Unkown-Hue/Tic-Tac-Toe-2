#include <stdio.h>

#include "engine.h"
#include "eval.h"
#include "cthread.h"
#include "game.h"
#include "tt.h"

#define LAZY_SORT_TOP 2

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

void SortlazyX(int x, int o, Move *move){
    int i;
    int j;
    for (i = 0; i < move->size; i++){
        move->score[i] = Smalleval(x | (1 << move->moves[i]), o);
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


void SortlazyO(int x, int o, Move *move){
    int i;
    int j;
    int top = move->size > LAZY_SORT_TOP ? LAZY_SORT_TOP : move->size;
    for (i = 0; i < move->size; i++){
        move->score[i] = Smalleval(o | (1 << move->moves[i]), x);
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

Tmoven Findmovethread(State *st, int wfree, int depth, uint64_t tablesize, int id){
    Tmoven tmove;
    Move move;
    int bestmove = -1;
    int score;
    int alpha = -999999;
    int bit;
    int beta = 999999;
    int nodes = 0;
    int i = 0;
    Ttable tt;
    Intt(&tt, tablesize);
    Extract(&move, wfree);
    if (st->move == X){
        SortX(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
        	Play(st, bit);
        	score = ttMinimax(st, &tt, alpha, beta, &nodes, depth - 1, 1, 0);
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
        	score = ttMinimax(st, &tt, alpha, beta, &nodes, depth - 1, 1, 1);
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
    tmove.nodes = nodes;
    free(tt.table);
    printf("\nTbhits: %llu\n", tt.hit);
    return tmove;
}

int Findmovet(State* st, int depth, int threadcount){
    Tmoven move = {st->move == X ? -999999 : 999999, 0, 0};
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
    free(thread.thread);
    return move.move;
}

Tmoven Findmovestruct(State *st, int depth){
    Tmoven tmove;
    Move move;
    int bestmove = -1;
    int score;
    int alpha = -999999;
    int bit;
    int beta = 999999;
    int nodes = 0;
    int wfree = All(st) ^ MAXBIT;
    int i = 0;
    Extract(&move, wfree);
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
            if (alpha >= beta) break;
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
            if (alpha >= beta) break;
            i++;
        }
    }
    //printf("nodes searched: %d from id: %d\n", nodes, id);
    tmove.move = bestmove;
    tmove.score = st->move == X ? alpha : beta;
    tmove.nodes = nodes;
    return tmove;
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

int ttMinimax(State *st, Ttable *tt, int alpha, int beta, int *nodes, int depth, int fulldepth, int ismax){
	*nodes = *nodes + 1;
    Key key = Gettablekey(st->hash, tt->size);
    if (tt->table[key].hash == st->hash && tt->table[key].depth >= fulldepth){
        tt->hit++;
        return tt->table[key].score;
    }
    if (Win(st, X)) {
        tt->table[key].hash = st->hash;
        tt->table[key].depth = fulldepth;
        tt->table[key].score = 1000 + depth * 20;
        return 1000 + depth * 20;
    }
    if (Win(st, O)) {
        tt->table[key].hash = st->hash;
        tt->table[key].depth = fulldepth;
        tt->table[key].score = -1000 - depth * 20;
        return -1000 - depth * 20;
    }
    if (Full(st)){
        tt->table[key].hash = st->hash;
        tt->table[key].depth = fulldepth;
        tt->table[key].score = 0;
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
        int eval = Eval(st->side[X], st->side[O]);
        tt->table[key].hash = st->hash;
        tt->table[key].score = eval;
        tt->table[key].depth = fulldepth;
		return eval;
	}
	// do maximizing and call mimimin
    Move move;
    int bestmove;
	int free = All(st) ^ MAXBIT;
	int bit;
    int score;
    int i = 0;
    int cutoff = 0;
    SIDE side = st->move;
    Extract(&move, free);
    if (ismax){
        SortX(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
   	        Play(st, bit);
	        score = ttMinimax(st, tt, alpha, beta, nodes, depth - 1, fulldepth + 1, 0);
	        Undo(st);
	        if (score > alpha){
	    	    alpha = score;
                bestmove = move.moves[i];
	        }
            if (beta <= alpha){
                cutoff = 1;
                break;
            }
            i++;
        }
        if (!cutoff){
            tt->table[key].hash = st->hash ^ Zobrist[st->move][bestmove];
            tt->table[key].score = alpha;
            tt->table[key].depth = depth;
        }
	    return alpha;
    }
    else {
        SortO(st->side[X], st->side[O], &move);
        while (i < move.size){
            bit = move.moves[i];
   	        Play(st, bit);
	    	score = ttMinimax(st, tt, alpha, beta ,nodes, depth - 1, fulldepth + 1, 1);
	    	Undo(st);
	    	if (score < beta){
	    		beta = score;
	    	}
            if (beta <= alpha){
                cutoff = 1;
                break;
            }
            i++;
        }
        if (!cutoff){
            tt->table[key].hash = st->hash ^ Zobrist[st->move][bestmove];
            tt->table[key].score = beta;
            tt->table[key].depth = depth;
        }
	    return beta;
    }
}
