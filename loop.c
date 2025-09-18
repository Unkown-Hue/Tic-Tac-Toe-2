#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cthread.h"
#include "game.h"
#include "engine.h"
#include "loop.h"


size_t tobit(char *str){
	size_t med = 1;
	size_t final = 0;
	int i = strlen(str) - 1;
	while (i >= 0 && isdigit(str[i])){
		if (isdigit(str[i])){
			final += ( str[i] - '0') * med;
		}
		med *= 10;
		i--;
	}
	return final;
}

char *Getline(){
	int ch;
	char *buffer;
	size_t buffersize;
	size_t n;
	n = 0;
	buffersize = 512;
	buffer = (char*)malloc(buffersize);
	while ((ch = getchar()) != EOF){
		if (n > buffersize){
			buffersize += 512;
			buffer = (char*)realloc(buffer, buffersize);
		}
		if (ch == '\n'){
			buffer[n] = '\0';
			return buffer;
		}
		buffer[n++] = ch;
	}
	buffer[n] = '\0';
	return buffer;
}

void PlayGame(){
	State st;
	SetState(&st);
	Initw();
	Tmoven move;
    char *line;
    int player;
    int depth = 8;
    int threadcount = Getthreadcount();
    //printf("Thread count %d\n", threadcount);
    clock_t start, end;
    start = clock();
    printf("Play as x or o [x/o]: ");
    while (1){
        char *newline = Getline();
        printf("\n");
        if (strcmp(newline, "x") == 0){
            player = 1;
            break;
        }
        if (strcmp(newline, "o") == 0){
            player = 0;
            break;
        }
        if (strcmp(newline, "X") == 0){
            printf("No.\n");
        }
        else if (strcmp(newline, "O") == 0){
            printf("No.\n");
        }
        free(newline);
    }
	Printboard(&st);
    if (player){
	    do {
            printf("enter a move: ");
            line = Getline();
            printf("\n");
            if (strcmp(line, "exit") == 0){
                break;
            }
            move.move = tobit(line);
            free(line);
            Play(&st, move.move);
	        move = ttFindmove(&st, depth);
	    	Play(&st, move.move);
	    	Printboard(&st);
	    	if (Winall(&st)){
	    		printf("Win Detected.\n");
               break;
	    	}
            printf("Bot played: %d with score: %d\n", move.move, move.score);
	    } while(All(&st) ^ MAXBIT);
    }
    else {
	    do {
	        move = ttFindmove(&st, depth);
	    	Play(&st, move.move);
	    	Printboard(&st);
            printf("Bot played: %d with score: %d\n", move.move, move.score);
	    	if (Winall(&st)){
	    		printf("Win Detected.\n");
               break;
	    	}
            printf("enter a move: ");
            line = Getline();
            printf("\n");
            if (strcmp(line, "exit") == 0){
                break;
            }
            move.move = tobit(line);
            free(line);
            Play(&st, move.move);
	    	if (Winall(&st)){
	    		printf("Win Detected.\n");
               break;
	    	}
	    } while(All(&st) ^ MAXBIT);
    }
    end = clock();
	printf("Done. time taken: %lu ms\n", end - start);
    return;
}

int Loop(){
    PlayGame();
    return 0;
}
