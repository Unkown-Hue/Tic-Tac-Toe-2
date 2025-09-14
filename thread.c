// thread.c
#include "cthread.h"
#include "engine.h"

#include <tlhelp32.h>
#include <stdio.h>

// array to hold all thread best moves and score.
Tmoven Movelist[2000];

int Getproccescount() {
    DWORD pid = GetCurrentProcessId();
    HANDLE hthreadsnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 te;
    te.dwSize = sizeof(THREADENTRY32);
    int count = 0;

    if (Thread32First(hthreadsnap, &te)) {
        do {
            if (te.th32OwnerProcessID == pid) {
                count++;
            }
        } while (Thread32Next(hthreadsnap, &te));
    }

    CloseHandle(hthreadsnap);
    return count;
}

int Getthreadcount() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}

DWORD WINAPI Threadedsearch(LPVOID lparam){
	Threadparam *param = (Threadparam*)lparam;
	Movelist[param->id] = Findmovethread(&param->st, param->amount, param->depth, param->id);
	free(param);
	return 0;
}

// function to initialize a thread.
void Inthread(Thread *thread){
	thread->size = 2;
	thread->thread = (HANDLE*)malloc(thread->size * sizeof(HANDLE));
}

// function to create threads.
void Createthreads(Thread *thread, State *st, int size, int depth){
	int i;
	int j;
	int free = All(st) ^ MAXBIT;
	int div = Count1s(free);
	int left;
	int amount;
	Threadparam *param;
	if (size == 0) return;
	if (div == 0) return;
	if (!(div / size)){
		do {
			size--;
		} while (!(div / size));
	}
	left = div % size;
	for (i = 0; i < size; i++){
		amount = 0;
        if (i == size - 1){
		    while (free){
		    	amount |= 1 << Poplsb(&free);
		    }
        }
        else {
		    for (j = 0; j < div / size; j++){
		    	amount |= 1 << Poplsb(&free);
		    }
        }
		if (i >= thread->size){
			thread->size += 1;
			thread->thread = (HANDLE*)realloc(thread->thread, thread->size * sizeof(HANDLE));
		}
		param = (Threadparam*)malloc(1 * sizeof(Threadparam));
		param->amount = amount;
		param->depth = depth;
		param->st = *st;
		param->id = i;
		thread->thread[i] = CreateThread(NULL, 0, (LPVOID)Threadedsearch, param,0, NULL);
	}
    WaitForMultipleObjects(thread->size, thread->thread, TRUE, INFINITE);
}
