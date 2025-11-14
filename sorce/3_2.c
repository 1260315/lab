/*
【課題 3-2】
複数のスレッドが共有データ(大域変数)を安全に操作するプログラムを作成する。
以下のリスト 2 は、2 つのスレッドが大域変数 counter を加算しているプログラムである。
func1を実行するスレッドは3ずつ加算し、func2を実行するスレッドは5ずつ加算するが、
スレッド間の排他制御がされていないため、誤動作するプログラムである。
ここでは、このプログラムを正しく動作させるため、ロック(pthread_mutex)を用いた排他制御を実現せよ。
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    // pthread

void *func1(void *param);
void *func2(void *param);
int counter = 0;

int main(void){
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, func1, NULL);
    pthread_create(&tid2, NULL, func2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    
}

void *func1(void *param){
    int i, tmp;
    for (i = 0; i < 10; i++)
    {
        tmp = counter;
        sleep(rand() % 4);
        tmp = tmp + 3;
        counter = tmp;
        printf("¥n i=%d counter -> %d", i, counter);

    }
    pthread_exit(0);

}

void *func2(void *param){
    int k, tmp;
    for (k = 0; k < 10; k++){
        tmp = counter;
        sleep(rand() % 4);
        tmp = tmp + 5;
        counter = tmp;
        printf("¥n k=%d counter -> %d", k, counter);

    }
    pthread_exit(0);
}