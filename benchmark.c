#include <time.h>

#include "cthread.h"
#include "game.h"
#include "engine.h"
#include "loop.h"

typedef unsigned long long uint64;


uint64 Perft(State *st, int depth){
    if (depth == 0 || Winall(st) || Full(st)) return 1;
    uint64 nodes = 0;
    int free = All(st) ^ MAXBIT;
    while (free){
        int move = Poplsb(&free);
        Play(st, move);
        nodes += Perft(st, depth - 1);
        Undo(st);
    }

    return nodes;
}

uint64 Perftalpha(State *st, int depth, int threadcount){
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
    return move.nodes;
}

int main(){
    State st;
    SetState(&st);
    Initw();
    int threadcount = Getthreadcount();
    int depth = 6;
    int depth2 = 11;
    clock_t start, end;
    start = clock();
    uint64 nodes = Perft(&st, depth);
    end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: regular perft\n");
    printf("Perft(%d) = %llu nodes\n", depth, nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n\n", nodes / elapsed);
    start = clock();
    nodes = Perftalpha(&st, depth2, threadcount);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: alpha beta perft\n");
    printf("Perft(%d) = %llu nodes\n", depth2, nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n", nodes / elapsed);
    getchar();
    return 0;
}
