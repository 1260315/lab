/*
【課題5】通信処理
〇処理モデル
・一度に複数のクライアントからの接続を処理するマルチクライアントモデルを実現する
・クライアントプロセスとサーバプロセス間の通信プロトコルは、TCP/IPを用いる
〇サーバプロセス
・サーバプロセスは、クライアントプロセスから文字列を受け取ると受け取ったメッセージを表示し、受領完了のメッセージを送信する
・シングルプロセス、シングルスレッドで実現する
〇クライアントプロセス
・クライアントプロセスは、自身を表す固有メッセージをサーバプロセスに送信する
〇その他
・複数のクライアントが同時に接続していることがわかる
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // getpid, read, wait
#include <sys/wait.h>   // wait

int main(void){
        int pid;        //fork
    int status;     //wait
    if((pid = fork()) < 0){         //forkで子プロセスの作成
        perror("fork");
        exit(1); 

    }else if(pid == 0){     //子プロセスである
        exit(0);

    }else{      //親プロセスである
        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        exit(0);
        
    }
}