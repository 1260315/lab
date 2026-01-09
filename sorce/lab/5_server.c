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
#include <sys/socket.h>     // socket, bind, listen, accept
#include <sys/select.h>     // select, FD_*, 
#include <string.h>         // memset, strlen
#include <arpa/inet.h>      // htons
#include <unistd.h>         // close, write, read
#include <sys/uio.h>        // write

#define SERVER_ADDRESS   "127.0.0./1"
#define SERVER_PORT     719
enum {
    BACKLOG = 5,         // 順番待ちの数
    BUFFER_SIZE = 1024,
    MAXCLIENTS = 10,     // 最大クライアント数
};

int clientSockDescs[MAXCLIENTS];
int clientsNum = 0;

void delete_client(int clientSocket){
    for(int i = 0; i < clientsNum; i++){
        if(clientSockDescs[i] == clientSocket){
            clientSockDescs[i] = clientSockDescs[clientsNum - 1];
            clientsNum--;
            break;
        }
    }
}



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

    // ソケットのオプション設定
    int optionValue = 1;
    if(setsockopt(serverSockDesc, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(optionValue)) == -1){
        perror("setsockopt");
        exit(1);
    }

    // ソケットのアドレスのデータを作る
    struct sockaddr_in serverSockAddr;

    memset(&serverSockAddr, 0, sizeof(serverSockAddr));
    //serverSockAddr.sin_len = sizeof(serverSockAddr);
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(SERVER_PORT);
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);


    // bind ソケットにアドレスを結びつける
    if(bind(serverSockDesc, (struct sockaddr *)&serverSockAddr, sizeof(serverSockAddr)) < 0){
        perror("bind");
        exit(1);
    }


    // 接続の受付開始
    if(listen(serverSockDesc, BACKLOG)){
        perror("listen");
        exit(1);
    }

    fprintf(stderr, "# サーバプロセス起動中...\n");

    for(;;){

        // select用の記述子集合を作る
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(serverSockDesc, &readFds);
        int maxFd = serverSockDesc;

        //クライアントからの受信を見張る
        for(int i = 0; i < clientsNum; i++){
            FD_SET(clientSockDescs[i], &readFds);
            if(clientSockDescs[i] > maxFd){
                maxFd = clientSockDescs[i];
            }
        }

        // selectで記述子の読み込み準備完了を待つ
        if(select(maxFd + 1, &readFds, NULL, NULL, NULL) < 0){
            perror("select");
            exit(1);
        }

        // 新しい接続か？
        if(FD_ISSET(serverSockDesc, &readFds)){

            // クライアントソケット用変数の宣言
            int clientSockDesc;
            struct sockaddr_in clientSockAddr;
            socklen_t clientSockAddrLen = sizeof(clientSockAddr);
            
            if((clientSockDesc = accept(serverSockDesc, (struct sockaddr *)&clientSockAddr, &clientSockAddrLen)) < 0){
                perror("accept");
                continue;
            };

            // 接続クライアント数に空きがあるか？
            if(clientsNum >= MAXCLIENTS){
                shutdown(clientSockDesc, SHUT_RDWR);
                close(clientSockDesc);
            
            }else{
                clientSockDescs[clientsNum] = clientSockDesc;
                clientsNum++;
                fprintf(stderr, "# 新クライアントと通信開始\n");
                fprintf(stderr, "# 現在の接続クライアント数：%d\n", clientsNum);
            }
        }

        // 処理
        for(int i=0; i < clientsNum; i++){
            if(FD_ISSET(clientSockDescs[i], &readFds)){

                // 読み書き用
                char outBuf[BUFFER_SIZE];
                int charNum;
                char message[] = "受信完了\n";

                // メッセージの読み込みと出力
                charNum = read(clientSockDescs[i], outBuf, sizeof(outBuf) - 1);
                
                if(charNum == -1){
                    perror("read");
                    close(clientSockDescs[i]);
                    delete_client(clientSockDescs[i]);
                    fprintf(stderr, "# クライアントとの通信終了（エラー）\n");
                    fprintf(stderr, "# 現在の接続クライアント数：%d\n", clientsNum);
                    continue;
                }else if(charNum == 0){
                    // クライアントが切断
                    close(clientSockDescs[i]);
                    delete_client(clientSockDescs[i]);
                    fprintf(stderr, "# クライアントが切断しました\n");
                    fprintf(stderr, "# 現在の接続クライアント数：%d\n", clientsNum);
                    continue;
                }

                // 受信したメッセージを表示
                outBuf[charNum] = '\0';
                fprintf(stderr, "[クライアント]%s", outBuf);
                fprintf(stderr, "# メッセージ受信完了\n");

                // クライアントへメッセージを送信
                if(write(clientSockDescs[i], message, strlen(message)) < 0){
                    perror("write");
                    close(clientSockDescs[i]);
                    delete_client(clientSockDescs[i]);
                    fprintf(stderr, "# クライアントとの通信終了（エラー）\n");
                    fprintf(stderr, "# 現在の接続クライアント数：%d\n", clientsNum);
                    continue;
                }

            }
        }
    }
}