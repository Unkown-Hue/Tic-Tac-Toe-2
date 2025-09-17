#include <time.h>

#include "cthread.h"
#include "engine.h"
#include "game.h"

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
        move.nodes += Movelist[i].nodes;
    }
    free(thread.thread);
    return move.nodes;
}

uint64 Perftalphaunthreaded(State *st, int depth, int threadcount){
    Tmoven move = Findmovethread(st, All(st) ^ MAXBIT, depth, TABLESIZE, 0);
    return move.nodes;
}

uint64 Perftalphanott(State *st, int depth){
    Tmoven move = Findmovestruct(st, depth);
    return move.nodes;
}

int main(){
    State st;
    SetState(&st);
    Initw();
    int threadcount = Getthreadcount();
    int depth = 3;
    int depth2 = 8;
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
    nodes = Perftalphaunthreaded(&st, depth2, threadcount);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: alpha beta perft\n");
    printf("Perft(%d) = %llu nodes\n", depth2, nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n\n", nodes / elapsed);
    start = clock();
    nodes = Perftalphanott(&st, depth2);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: alpha beta with no tablebase perft\n");
    printf("Perft(%d) = %llu nodes\n", depth2, nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n", nodes / elapsed);
    getchar();
    return 0;
}
