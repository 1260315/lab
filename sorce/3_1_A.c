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
#include <sys/types.h>  // getpid, read, wait
#include <sys/uio.h>    // read, write
#include <sys/wait.h>   // wait
#include <unistd.h>     // getpid, close, pipe, read, write

int counter = 0;     //大域変数の初期化

int main(void){ 

    int pid;        //fork
    int status;     //wait

    if((pid = fork()) < 0){         //forkで子プロセスの作成
        perror("fork");
        exit(1);    

    }else if(pid == 0){     //子プロセスである
        counter += 10;
        exit(0);

    }else{      //親プロセスである
        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        printf("大域変数 : %d\n", counter);      //大域変数の表示
        exit(0);
    }
}