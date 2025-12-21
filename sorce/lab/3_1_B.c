/*
【課題 3-1】
fork システムコールを利用し子プロセスを生成するプログラムと、
pthread_create 関数を使って子スレッドを生成するプログラムを作成する。

A. プロセスを生成するプログラムは以下の仕様である。
(1) 大域変数を 0 に初期化する。
(2) 子プロセスを生成し、子プロセス側で大域変数を 10 加算する。
(3) 親プロセスは子プロセスの終了を待ち、大域変数の値を表示する。

B.スレッドを生成するプログラムは以下の仕様である。
(a) 大域変数を 0 に初期化する。
(b) (子)スレッドを生成し、スレッド側で大域変数を 10 加算する。
(c) 親プロセスはスレッドの終了を待ち、大域変数の値を表示する。
(d) 使用するスレッド関連の関数は、pthread_create, pthread_join, pthread_exit である。
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <pthread.h>    // pthread


int counter = 0;     //大域変数の初期化

void * threadFunc(void *arg);

int main(void){ 
    pthread_t thread;

    //子スレッドの作成
    if(pthread_create(&thread, NULL, threadFunc, NULL) != 0){
        perror("pthread_creat");
        exit(1);

    }

    //子スレッドの終了を待つ
    if(pthread_join(thread, NULL) != 0){
        perror("pthread_join");
        exit(1);

    }

    //大域変数を出力
    printf("大域変数 : %d\n", counter);
    
    exit(0);

}

void *threadFunc(void *arg){
    counter += 10;
    pthread_exit(NULL);
}