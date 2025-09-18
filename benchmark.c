#include <time.h>

#include "cthread.h"
#include "engine.h"
#include "game.h"

typedef unsigned long long uint64;

typedef struct {
    int move;
    uint64 nodes;
} Moveinfo;

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

Moveinfo Perftalpha(State *st, int depth, int threadcount){
    Moveinfo moveinfo;
    Tmoven move = {st->move == X ? -999999 : 999999, 0, 0};
    Thread thread;
    Inthread(&thread);
    Createthreads(&thread, st, threadcount, depth);
    for (int i = 0; i < threadcount; i++){
        move.nodes += Movelist[i].nodes;
    }
    free(thread.thread);
    moveinfo.move = move.move;
    moveinfo.nodes = move.nodes;
    return moveinfo;
}

Moveinfo Perftalphaunthreaded(State *st, int depth, int usett){
    Tmoven move = Findmovethread(st, All(st) ^ MAXBIT, depth, TABLESIZE, usett, 0);
    Moveinfo moveinfo;
    moveinfo.move = move.move;
    moveinfo.nodes = move.nodes;
    return moveinfo;
}

Moveinfo Perftalphanott(State *st, int depth){
    Moveinfo moveinfo;
    Tmoven move = Findmove(st, depth);
    moveinfo.move = move.move;
    moveinfo.nodes = move.nodes;
    return moveinfo;
}

Moveinfo Perftalphava(State *st, int depth){
    Moveinfo moveinfo;
    Tmoven move = Findmovestruct(st, depth);
    moveinfo.move = move.move;
    moveinfo.nodes = move.nodes;
    return moveinfo;
}

int main(){
    State st;
    Moveinfo minfo;
    SetState(&st);
    Initw();
    int threadcount = Getthreadcount();
    int depth = 8;
    clock_t start, end;
    double elapsed;
    start = clock();
    minfo = Perftalphanott(&st, depth);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: alpha beta itterave deppening\n");
    printf("Perft(%d) = %llu nodes\n", depth, minfo.nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n\n", minfo.nodes / elapsed);
    printf("Best move: %d\n\n", minfo.move);
    // another
    start = clock();
    minfo = Perftalphava(&st, depth);
    end = clock();
    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Perft: alpha beta with no tablebase perft\n");
    printf("Perft(%d) = %llu nodes\n", depth, minfo.nodes);
    printf("Time: %.3f seconds\n", elapsed);
    printf("Speed: %.2f nodes/sec\n\n", minfo.nodes / elapsed);
    printf("Best move: %d\n\n", minfo.move);
    return 0;
}
