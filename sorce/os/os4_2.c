/*
os 第4回課題

2. マルチスレッド処理
o親スレッド
    書き込みが終了したことを示す大域変数flagを0に初期化する
    子スレッドをpthread_createにより生成
    標準入力より、255B以下のメッセージを受け取り、大域変数buffに書き込む
    書き込みが終了したことをflagを1に変更して示す
    子スレッドの終了を待ち、終了する
o子スレッド
    書き込みが終了したことを示すflagが1になるまで無限の空ループを実行する。
    flagが1になると、buffからメッセージを読み込み、標準出力に表示する
    終了
*/
#include <stdio.h>      
#include <stdlib.h>     // exit
#include <pthread.h>    // pthread

//大域変数
char buff[255];
int flag;

//スレッドの定義
void * threadFunc(void *arg);

int main(void){

    //flagを0に初期化
    flag = 0;   

    pthread_t thread;

    //子スレッドの作成
    if(pthread_create(&thread, NULL, threadFunc, NULL) != 0){
        perror("pthread_creat");
        exit(1);

    }

    //標準入力からメッセージを受け取る
    fprintf(stdout, "メッセージを入力してください(255B以下)：");
    fscanf(stdin, "%s", buff);

    //書き込み終了
    flag = 1;

    //子スレッドの終了を待つ
    if(pthread_join(thread, NULL) != 0){
        perror("pthread_join");
        exit(1);

    }
    
    exit(0);

}

void *threadFunc(void *arg){
    for(;flag != 1;){};

    fprintf(stdout, "メッセージ：%s\n", buff);
    exit(0);
}