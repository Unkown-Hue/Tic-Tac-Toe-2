#include "eval.h"
#include "game.h"

#define SCOREAMOUNT 3
#define WEAK3 10
#define THREAT3 25
#define THREAT4 250
#define STRONG4 410
#define THREAT5 650

const int boardbias[25][2] = {
    1, 3,
    2, 1,
    4, 1,
    8, 1,
    16, 3,
    32, 2,
    64, 1,
    128, 3,
    256, 4,
    512, 1,
    1024, 1,
    2048, 3,
    4096, 5,
    8192, 3,
    16384, 2,
    32768, 2,
    65536, 2,
    131072, 2,
    262144, 2,
    524288, 1,
    1048576, 3,
    2097152, 1,
    4194304, 2,
    8388608, 1,
    16777216, 3
};

int Bigeval(int x, int o, SIDE side){
	int score = 0;
	int bits;
    int multilinex = 0;
    int totalx = 0;
    int multilineo = 0;
    int totalo = 0;
	for (int i = 0; i < MAXWIN; i++){
		bits = x & Warray[i];
        int xcount = Count1s(x & Warray[i]);
        int ocount = Count1s(o & Warray[i]);
        if (xcount == 4 && ocount == 0){
			score += THREAT5;
		}
        else if (xcount == 3 && ocount == 0 && side == X){
			score += THREAT5;
		}
        else if (xcount == 3 && ocount == 0){
			score += THREAT4;
            multilinex++;
            totalx += xcount;
		}
        else if (xcount == 2 && ocount == 0 && side == X){
			score += THREAT4;
            multilinex++;
            totalx += xcount;
		}
        else if (xcount == 2 && ocount == 0){
			score += THREAT3;
            multilinex++;
            totalx += xcount;
		}
        if (ocount == 4 && xcount == 0){
			score += THREAT5;
		}
        else if (ocount == 3 && xcount == 0 && side == O){
			score += THREAT5;
		}
        else if (ocount == 3 && xcount == 0){
			score += THREAT4;
            multilineo++;
            totalo += ocount;
		}
        else if (ocount == 2 && xcount == 0 && side == O){
			score += THREAT4;
            multilineo++;
            totalo += ocount;
		}
        else if (ocount == 2 && xcount == 0){
			score += THREAT3;
            multilineo++;
            totalo += ocount;
		}
	}
    while (x){
        int bit = Poplsb(&x);
        score += boardbias[bit][1];
        if (o){
            bit = Poplsb(&o);
            score -= boardbias[bit][1];
        }
    }
    if (multilinex == 2 && totalx == 4) score += THREAT4;
    else if (multilinex == 2 && totalx == 3) score += WEAK3;
    else if (multilinex == 2 && totalx == 5) score += STRONG4;
    else if (multilinex == 3 && totalx == 3) score += THREAT3;
    else if (multilinex == 3 && totalx == 5) score += STRONG4;
    else if (multilinex == 3 && totalx == 4) score += THREAT3;
    else if (multilinex == 4 && totalx > 4) score += THREAT4;

    if (multilineo == 2 && totalo == 4) score += THREAT4;
    else if (multilineo == 2 && totalo == 3) score -= WEAK3;
    else if (multilineo == 2 && totalo == 5) score -= STRONG4;
    else if (multilineo == 3 && totalo == 3) score -= THREAT3;
    else if (multilineo == 3 && totalo == 5) score -= STRONG4;
    else if (multilineo == 3 && totalo == 4) score -= THREAT3;
    else if (multilineo == 4 && totalo > 4) score -= STRONG4;
	return score;
}

int Eval(int x, int o){
	int score = 0;
	int bits;
	for (int i = 0; i < MAXWIN; i++){
		bits = x & Warray[i];
        int xcount = Count1s(x & Warray[i]);
        int ocount = Count1s(o & Warray[i]);
        if (xcount == 4 && ocount == 0){
			score += THREAT5;
		}
        else if (xcount == 3 && ocount == 0){
			score += THREAT4;
		}
        else if (xcount == 2 && ocount == 0){
			score += THREAT3;
		}
        if (ocount == 4 && xcount == 0){
			score -= THREAT5;
		}
        if (ocount == 3 && xcount == 0){
			score -= THREAT4;
		}
        if (ocount == 2 && xcount == 0){
			score -= THREAT3;
		}
	}
    while (x){
        int bit = Poplsb(&x);
        score += boardbias[bit][1];
        if (o){
            bit = Poplsb(&o);
            score -= boardbias[bit][1];
        }
    }
	return score;
}

int Smalleval(int x, int o){
	int score = 0;
	int bits;
	for (int i = 0; i < MAXWIN; i++){
		bits = x & Warray[i];
        int xcount = Count1s(x & Warray[i]);
        int ocount = Count1s(o & Warray[i]);
        if (xcount == 4 && ocount == 0){
			score += THREAT5;
		}
        else if (xcount == 3 && ocount == 0){
			score += THREAT4;
		}
        else if (xcount == 2 && ocount == 0){
			score += THREAT3;
		}
        if (ocount == 4 && xcount == 0){
			score -= THREAT5;
		}
        if (ocount == 3 && xcount == 0){
			score -= THREAT4;
		}
        if (ocount == 2 && xcount == 0){
			score -= THREAT3;
		}
	}
	return score;
}
