#include <stdio.h>
#include <stdlib.h>

int main(){
    int * mal_p;
    mal_p = (int *)malloc(sizeof(int));
    printf("コード領域：%p\n", main);
    printf("ヒープ領域：%p\n", mal_p);
}

