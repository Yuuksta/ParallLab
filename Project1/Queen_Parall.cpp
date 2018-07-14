#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <malloc.h>

#define N 14
using namespace std;

typedef struct THREAD_HANDLE_T {
    int threadNum;
} THREAD_HANDLE_T;

static int chessBoard[N][N] = { 0 };
static int Count[N] = { 0 };
static int totalCount = 0;
pthread_t tid[N];


void print(int chessBoardNum);
int isValid(int loop, int value, int chessBoardNum);
void Queens(int index, int chessBoardNum);
void *process(void *args);

int main(int argc, char*argv[]) {
    int i, j;
    float deltaT;
    struct timeval t1, t2;
    //create threads
    for(i = 0; i < N; i++) {
        THREAD_HANDLE_T *arg = (THREAD_HANDLE_T *)malloc(sizeof(THREAD_HANDLE_T));
        arg->threadNum = i;
        if(pthread_create(&tid[i], NULL, process, arg)) {
            printf("create pthread error!\n");
            return -1;
        }
    }

    //start timer
    gettimeofday(&t1, NULL);

    //pthread join
    for(i = 0; i < N; i++) {
        pthread_join(tid[i], NULL);
    }

    for(i = 0; i < N; i++) {
        totalCount += Count[i];
    }
    
    cout << "total=" << totalCount << endl;
    //stop timer
    gettimeofday(&t2, NULL);
    deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
    deltaT = deltaT / 1000000;
    printf("time elapsed: %f s\n", deltaT);
    return 0;
}


void print(int chessBoardNum) {
    for (int i = 0; i < N ; i++) {   
        int inner;
        for (inner = 0; inner < chessBoard[chessBoardNum][i]; inner++) cout << "- ";
        cout <<"# ";
        for (inner = chessBoard[chessBoardNum][i] + 1; inner < N ; inner++) cout << "- ";
        cout << endl;
    }
    cout << "______________________\n";
}

int isValid(int loop, int value, int chessBoardNum) {
    int index;
    int data;
    for (index = 0; index < loop; index++) {
        data = chessBoard[chessBoardNum][index];
        if (value == data)
            return 0;
        if ((index + data) == (loop + value))
            return 0;
        if ((index - data) == (loop - value))
            return 0;
    }
    return 1;
}

void Queens(int index, int chessBoardNum) {
    int loop;
    for (loop = 0; loop < N ; loop++) {
        if (isValid(index, loop, chessBoardNum)) {
            chessBoard[chessBoardNum][index] = loop;
            if (N  - 1 == index) {
                Count[chessBoardNum]++;
                //print(chessBoardNum);
                chessBoard[chessBoardNum][index] = 0;
                return;
            }
            Queens(index + 1, chessBoardNum);
            chessBoard[chessBoardNum][index] = 0;
        }
    }
}

void *process(void *arg) {
    THREAD_HANDLE_T *args = (THREAD_HANDLE_T *)arg;
    chessBoard[args->threadNum][0] = args->threadNum; 
    Queens(1, args->threadNum);
}