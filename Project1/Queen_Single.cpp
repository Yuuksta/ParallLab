#include <iostream>
#include <stdio.h>
#include <sys/time.h>

#define N 14

using namespace std;

static int gEightQueen[N] = { 0 }, gCount = 0;
void print()//输出每一种情况下棋盘中皇后的摆放情况
{
    for (int i = 0; i < N; i++)
    {   
        int inner;
        for (inner = 0; inner < gEightQueen[i]; inner++)
            cout << "0";
            cout <<"#";
        for (inner = gEightQueen[i] + 1; inner < N; inner++)
            cout << "0";
        cout << endl;
    }
    cout << "==========================\n";
}
int check_pos_valid(int loop, int value)//检查是否存在有多个皇后在同一行/列/对角线的情况
{
    int index;
    int data;
    for (index = 0; index < loop; index++)
    {
        data = gEightQueen[index];
        if (value == data)
            return 0;
        if ((index + data) == (loop + value))
            return 0;
        if ((index - data) == (loop - value))
            return 0;
    }
    return 1;
}
void eight_queen(int index)
{
    int loop;
    for (loop = 0; loop < N; loop++)
    {
        if (check_pos_valid(index, loop))
        {
            gEightQueen[index] = loop;
            if (N - 1 == index)
            {
                gCount++;
                //print();
                gEightQueen[index] = 0;
                return;
            }
            eight_queen(index + 1);
            gEightQueen[index] = 0;
        }
    }
}
int main(int argc, char*argv[])
{
    float deltaT;
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    eight_queen(0);
    cout << "total=" << gCount << endl;
    gettimeofday(&t2, NULL);
    deltaT = (t2.tv_sec-t1.tv_sec) * 1000000 + t2.tv_usec-t1.tv_usec;
    deltaT = deltaT / 1000000;
    printf("time elapsed: %f s\n", deltaT);
    return 0;
}