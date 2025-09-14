// game.c
#include "game.h"

int Warray[28];
const int *pW = Warray;

// function to get if the game is full or not.
int Full(const State *st){
	return ((st->side[X] | st->side[O]) ^ MAXBIT) == 0;
}

// function to get all bits.
int All(const State *st){
	return st->side[X] | st->side[O];
}

void Initw(){
	int i = 0;
	int j = 15;
	int k;
	while (i < 10){
		Warray[i] = j;
		i++;
		j <<= 1;
		if (i < 10){
			Warray[i] = j;
			j <<= 4;
			i++;
		}
	}
	j  = 1;
	j |= 1 << 5;
	j |= 1 << 10;
	j |= 1 << 15;
	while (i < 20){
		k = j;
		Warray[i] = k;
		i++;
		k <<= 5;
		if (i < 20){
			Warray[i] = k;
		}
		k = j << 4;
		i++;
		j <<= 1;
	}
	j = 1 << 5;
	j |= 1 << 11;
	j |= 1 << 17;
	j |= 1 << 23;
	k = j;
	Warray[i++] = k;
	k <<= 1;
	Warray[i++] = k;
	k >>= 5;
	Warray[i++] = k;
	k >>= 1;
	Warray[i++] = k;

	j = 1 << 9;
	j |= 1 << 13;
	j |= 1 << 17;
	j |= 1 << 21;
	k = j;
	Warray[i++] = k;
	k >>= 1;
	Warray[i++] = k;
	k >>= 5;
	Warray[i++] = k;
	k <<= 1;
	Warray[i++] = k;
}

// basically a constructor. to set defualt value at start.
void SetState(State *st){
	st->side[X] = 0;
	st->side[O] = 0;
	st->move = X;
	st->mcount = 0;
}

// function to play a move from int and make sure its legal.
void Play(State *st, const int m){
	st->side[st->move] |= 1 << m;
	st->move ^= O;
	st->Marray[st->mcount] = 1 << m;
	st->mcount++;
}

// function to play a move from int and make sure its legal.
int Playr(State *st, const int m){
	if ((1 << m) & All(st)){
		return 0;
	}
	st->side[st->move] |= 1 << m;
	st->move ^= O;
	st->Marray[st->mcount] = 1 << m;
	st->mcount++;
    return 1;
}

int Win(const State *st, enum SIDE side){
	const int *bits = &st->side[side];
	for (int i = 0; i < MAXWIN; i++){
		if ((*bits & pW[i]) == pW[i]){
			return 1;
		}
	}
	return 0;
}

int Winall(const State *st){
	const int *bitsx = &st->side[X];
	const int *bitso = &st->side[O];
	for (int i = 0; i < MAXWIN; i++){
		if ((*bitsx & pW[i]) == pW[i]){
			return 1;
		}
		if ((*bitso & pW[i]) == pW[i]){
			return 1;
		}
	}
	return 0;
}

void Undo(State *st){
	st->mcount--;
	st->move ^= O;
	st->side[st->move] ^= st->Marray[st->mcount];
}

int Poplsb(int *n){
	int bit;
	for (bit = 0; bit < 25; bit++){
		if ((1 << bit) & *n) break;
	}
	*n ^= 1 << bit;
	return bit;
}

//prints the game.
void Printboard(State *st){
	char ch;
	printf("Board\n");
	for (int i = 0; i < MAXGRID; i++){
		if (i % 5 == 0 && i != 0){
			printf("\n-------------------\n");
		}
		if (st->side[X] & 1 << i){
			if (i % 5 != 0){
				printf("|");
			}
			printf(" X ");
		}
		else if (st->side[O] & 1 << i){
			if (i % 5 != 0){
				printf("|");
			}
			printf(" O ");
		}
		else {
			if (i % 5 != 0){
				printf("|");
			}
			printf("   ");
		}
	}
	printf("\n");
}

//prints the game.
void PrintboardNum(State *st){
	char ch;
	printf("Board\n");
	for (int i = 0; i < MAXGRID; i++){
		if (i % 5 == 0 && i != 0){
			printf("\n-------------------\n");
		}
		if (st->side[X] & 1 << i){
			if (i % 5 != 0){
				printf("|");
			}
			printf(" X ");
		}
		else if (st->side[O] & 1 << i){
			if (i % 5 != 0){
				printf("|");
			}
			printf(" O ");
		}
		else {
			if (i % 5 != 0){
				printf("|");
			}
			printf(" %d ", i);
		}
	}
	printf("\n");
}
