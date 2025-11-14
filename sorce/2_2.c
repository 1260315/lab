/*
【課題 2-2】
exec システムコールを利用し、実行するプログラムを変更する。
以下の仕様を満たすプログラム実行プログラムを作成せよ。
(1) 実行するプログラムは、実行時に引数で与える。
(2) 親プロセスが、fork システムコール用いて、子プロセスを生成する。
(3) 子プロセスは、C 言語演習で作成したプログラムを、exec システムコールを発行することで実行する。
(4) 親プロセスは、子プロセスの終了を待ち合わせて終了する。
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // getpid, read, wait
#include <sys/uio.h>    // read, write
#include <sys/wait.h>   // wait
#include <unistd.h>     // getpid, close, pipe, read, write

int main(int args, char *argv[]){

    pid_t pid;
    int status;

    if((pid = fork()) < 0){     //forkで子プロセｓ作成
        perror("fork");
        exit(1);

    }else if(pid == 0){         //子プロセスである
        execv("./14", argv);    //exec関数で2_2_excecd.cでプロセスを置き換える
        printf("エラー : プロセスがexecによって書き換えられていない\n");
        exit(1);

    }else{                      //親プロセスである
        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        exit(0);                            
    }
}