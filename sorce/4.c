/*
【課題4】優先度つきスケジューリング
Linuxでは優先度とラウンドロビンを組み合わせたスケジューリングが行われている。指定できる優先
度の範囲は、-20から19まであり、値の小さい方が、優先度が高い。本演習では、以下のプログラムを
作成し、優先度の違いが実行時間に与える影響について評価する。
親プロセスと子プロセスへ異なる優先度を設定し、処理実行時間を計測するプログラムを作成し、それ
ぞれの実行時間を表示せよ。なお、計測対象の処理は、空ループを一定回数実行するものとし、優先度
の設定はniceシステムコール、時間の計測はclock_gettimeを利用する。また、仮想マシンのCPU数は1
に設定する。　

(1) 親プロセスが開始時刻を取得し、子プロセスを生成する。
(2) 親プロセスは、優先度を-19に設定する。子プロセスの優先度を19に設定する。
(3) 親プロセスと子プロセスは空ループ処理100万回を実行する。
(4) 親プロセスと子プロセスは終了時刻を取得する。
(5) 終了時刻と開始時刻の差を求め、実行時間を表示する。
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // getpid, wait
#include <sys/wait.h>   // wait
#include <unistd.h>     // nice
#include <time.h>       // clock_gettime
#include <errno.h>      // errno

int main(void){

    //開始時刻の取得
    /*
    struct timespec {
    time_t   tv_sec;     　秒
    long     tv_nsec;      ナノ秒
    };
    */
    struct timespec before_ts;

    clock_gettime(CLOCK_REALTIME, &before_ts);
    printf("開始時刻(tv_nsec) : %ld\n", before_ts.tv_nsec);

    pid_t pid;
    int status;
    int priority;

    if((pid = fork()) < 0){           //forkで子プロセスの作成
        perror("fork");
        exit(1);

    }else if(pid == 0){     //子プロセスである

        //優先度設定-19に
        if((priority = nice(19)) != 19){
            perror("nice");
        };

        //空ループ
        for(int i = 0; i <= 1000000; i++){}

        //終了時刻を取得
        struct timespec after_ts;
        clock_gettime(CLOCK_REALTIME, &after_ts);

        //終了時間・実行時間を出力
        printf("\n子プロセス優先度 : %d\n子プロセス終了時刻(tv_nsec) : %ld\n子プロセス実行時間(nsec) : %ld\n", 
            priority, after_ts.tv_nsec, after_ts.tv_nsec-before_ts.tv_nsec);

        exit(0);

    }else{                  //親プロセスである

        //優先度設定19に
        if((priority = nice(-19)) != -19){
            perror("nice");
        };

        //空ループ
        for(int i = 0; i <= 1000000; i++){}

        //終了時刻を取得
        struct timespec after_ts;
        clock_gettime(CLOCK_REALTIME, &after_ts);

        //終了時間・実行時間を出力
        printf("\n親プロセス優先度 : %d\n親プロセス終了時刻(tv_nsec) : %ld\n親プロセス実行時間(nsec) : %ld\n", 
            priority, after_ts.tv_nsec, after_ts.tv_nsec-before_ts.tv_nsec);

        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        exit(0);
    }

}