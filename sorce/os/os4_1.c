/*
os 第4回課題

1．マルチプロセス処理
o親プロセス
    パイプの作成、
    forkによる子プロセスの作成、
    標準入力より255B以下のメッセージを受け取りパイプに書き込む、
    子プロセスの終了を待ち、終了
o子プロセス
    パイプからメッセージを読み込む、
    読み込んだメッセージを標準出力に表示
    終了
*/

#include <stdio.h>      // printf
#include <stdlib.h>     // exit
#include <sys/types.h>  // read, wait
#include <sys/uio.h>    // read, write
#include <sys/wait.h>   // wait
#include <unistd.h>     // close, pipe, read, write

int main(void){

    pid_t pid;          //fork用
    int status;         //wait用
    
    int fd[2], nbytes;  //pipe用
    char buff[255];

    if(pipe(fd) == -1){     //pipeの作成
        perror("pipe");
        exit(1);

    }
    
    if((pid = fork()) < 0){ //forkで子プロセスの作成
        perror("fork");
        exit(1);

    }else if(pid == 0){     //子プロセスである
        close(fd[1]);                                 //書き込み口のファイル記述子を閉じる
        
        nbytes = read(fd[0], buff, sizeof(buff));     //読み込み口からbuffに内容を読み込む
        fprintf(stdout, "メッセージ：%s\n", buff);     //読み込んだメッセージを標準出力に表示  
        
        close(fd[0]);           //読み込み口から読み込みが終わったので、ファイル記述子を閉じる

        exit(0);

    }else{                  //親プロセスである
        close(fd[0]);   //読み込み口のファイル記述子を閉じる

        fprintf(stdout, "メッセージを入力してください(255B以下)：");    //標準入力からメッセージを受け取る
        fscanf(stdin, "%s", buff);
        
        write(fd[1], buff, sizeof(buff));      //書き込み口に受け取ったメッセージbuffを書き込む
        
        close(fd[1]);   //書き込み口への書き込みが終わったので、ファイル記述子を閉じる
        
        if(wait(&status) == (pid_t) - 1){   //子プロセスの終了を待つ
            perror("wait");
            exit(1);
        }
        
        exit(0);

    }

}