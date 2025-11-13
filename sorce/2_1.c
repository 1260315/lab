/*
【課題 2-1】
fork システムコールを利用し、子プロセスを生成するプログラムを作成する。以下の仕様
を満たすプロセス生成プログラムを作成せよ。
(1) fork 後、親プロセスと子プロセスの両方で自身のプロセス id(pid)を表示させる。また、
pipe を使って、親プロセスから子プロセスへ文字列を送信し表示させること。
(2)親プロセスは、子プロセスの終了を待って、終了すること。
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // getpid, read, wait
#include <sys/uio.h>    // read, write
#include <sys/wait.h>   // wait
#include <unistd.h>     // getpid, close, pipe, read, write

int main(){

    pid_t pid;
    int status;
    
    int fd[2], nbytes;
    char buf[1024];

    if(pipe(fd) == -1){     //pipeの作成
        perror("pipe");
        exit(1);
    }


    if((pid = fork()) < 0){           //forkで子プロセスの作成
        exit(1);

    }else if(pid == 0){     //子プロセスである
        printf("子プロセスのpid : %d\n", getpid());     //自身のPIDを出力

        close(fd[1]);   //書き込み口のファイル記述子を閉じる
        nbytes = read(fd[0], buf, sizeof(buf));     //読み込み口からbufに内容を書き込む
        write(1, buf, nbytes);      //標準出力にbufの中身を出力
        close(fd[0]);   //pipeの読み込みが終わったので、読み込み口のファイル記述子を閉じる

        exit(0);

    }else{                  //親プロセスである
        printf("親プロセスのpid : %d\n", getpid());     //自身のPIDを出力

        close(fd[0]);   //読み込み口のファイル記述子を閉じる
        char text[] = "親２子\n";
        write(fd[1], &text, sizeof(text));      //書き込み口に文字列textを書き込む
        close(fd[1]);   //pipeへの書き込みが終わったので、書き込み口のファイル記述子を閉じる
        
        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        exit(0);
    }
    
}