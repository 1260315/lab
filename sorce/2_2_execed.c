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

int main(int args, char *argv[]){
    for(int i = 0; i < args; i++){
        printf("引き数%d : %s\n", i, argv[i]);
    }
    
    //scanf, open , write, close

    exit(0);
}