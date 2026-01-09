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

#include <stdio.h>          // fprintf, perror
#include <stdlib.h>         // exit
#include <sys/socket.h>     // socket, bind, connect
#include <string.h>         // memset, strlen
#include <arpa/inet.h>      // htons
#include <unistd.h>         // close, write, read, getpid
#include <sys/uio.h>        // write

#define SERVER_ADDR     "127.0.0.1"
#define SERVER_PORT     719

enum {
    BUFFER_SIZE = 1024,
};

int main(void){

    /* socket ソケットを作る
    通信領域：AF_INET(IPv4を基にしたTCP/IP通信)
    ケットの型：SOCK_STREAM(コネクション型通信)
    通信プロトコルのプロトコル番号：0()
    */
    int serverSockDesc;

    if((serverSockDesc = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }


    // サーバのソケットのアドレスのデータを作る
    struct sockaddr_in serverSockAddr;

    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    //serverSockAddr.sin_len = sizeof(serverSockAddr);
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(SERVER_PORT);
    serverSockAddr.sin_addr.s_addr = inet_addr(SERVER_ADDR);


    // connect サーバに接続
    fprintf(stderr, "# 接続中...\n");
    if(connect(serverSockDesc, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) < 0){
        perror("connect");
        exit(1);
    }
    fprintf(stderr, "# 通信開始\n");

    // 読み書き用
    char buf[BUFFER_SIZE];
    int charNum;
    pid_t pid = getpid();   // 固有のメッセージ(プロセス番号)
    
    // プロセスIDをサーバへ送信
    sprintf(buf, "pid:%d\n", pid);
    if(write(serverSockDesc, buf, strlen(buf)) < 0){
        perror("write");
        exit(1);
    }

    fprintf(stderr, "# メッセージ送信完了\n");

    // サーバからの応答を受信
    fprintf(stderr, "[サーバ]");
    if((charNum = read(serverSockDesc, buf, sizeof(buf))) > 0){
        write(1, buf, charNum);
    }
    if(charNum < 0){
        perror("read");
        exit(1);
    }

    fprintf(stderr, "# 接続を維持しています。終了するには Enter キーを押してください。\n");
    
    // Enterキーを待つ
    getchar();  

    // ソケットを閉じる
    close(serverSockDesc);
    exit(0);
}