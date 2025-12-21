/*
【問 14 改】scanf 関数を用いて、「あなたの名前は?」と表示したのちに名前を入力する。
その後、「あなたの年齢は?」と表示したのちに年齢を入力する。
最後に、名前->改行->年齢の順で result.txt に書き込みを行う。(write 関数を用いるとこ)
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // getpid, read, wait
#include <sys/uio.h>    // read, write
#include <sys/wait.h>   // wait
#include <unistd.h>     // getpid, close, pipe, read, write
#include <fcntl.h>      //open
#include <string.h>     //strlens

int main(int args, char *argv[]){

    //PIDを確認
    printf("exec後の子プロセスのPID : %d\n", getpid());
    
    char buf[1024];
    int fd;     //書き込み先のファイル記述子

    //ファイル記述子を開く
    if((fd = open("./result.txt", O_WRONLY|O_TRUNC|O_CREAT, 0666)) == -1){
        perror("open");
        exit(1);
    }

    //名前の入力・書き込み
    printf("あなたの名前は？\n");
    scanf("%s", buf);

    if(write(fd, buf, strlen(buf)) != strlen(buf)){
        perror("write");
        exit(1);
    }

    //改行の書き込み
    if(write(fd, "\n", 1) != 1){
        perror("write");
        exit(1);
    }

    //年齢の入力・書き込み
    printf("あなたの年齢は？\n");
    scanf("%s", buf);

    if(write(fd, buf, strlen(buf)) != strlen(buf)){
        perror("write");
        exit(1);
    }

    //ファイル記述子を閉じる
    if(close(fd) == -1){
        perror("close");
        exit(1);
    };

    exit(0);
}