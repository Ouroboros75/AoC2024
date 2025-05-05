#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char arr[10000][10000] = { 0 };
int updates[10000] = { 0 };

int main(int argc, char* argv[]){

    unsigned int used = 0;
    int c, curNum = 0, prevNum = 0, hold = 0, add = 0;
    long sum = 0, sum2 = 0;
    FILE *handlerStream;
    int i, j;

    handlerStream = fopen(argv[1], "r");
    while((c=getc(handlerStream))!='\n'){
        ungetc(c, handlerStream);
        if(!fscanf(handlerStream, "%d|%d%*c", &prevNum, &curNum)) return 0;
        arr[prevNum][curNum] = 1;
    }

    /*
    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++) if(arr[i][j]==1) printf("%d|%d\n", i, j);

    }
    */

    while((c=getc(handlerStream))!=EOF){
        ungetc(c, handlerStream);
        if(!fscanf(handlerStream, "%d", &(updates[used]))) return 0;
        for(used=1; getc(handlerStream)==','; used++){
            if(!fscanf(handlerStream, "%d", &(updates[used]))) return 0;
        }
        //for(int i=0; i<used; i++){
        //    printf("%d,", updates[i]);
        //}
        //printf("\n%d\n", used);
        add=updates[used/2];
        for(i=0; i<used; i++){
            for(j=i+1; j<used; j++){
                if(arr[updates[j]][updates[i]]!=0){
                    add = 0;
                    hold = updates[j];
                    updates[j] = updates[i];
                    updates[i] = hold;
                }
            }
            if(i==(used/2)+1) break;
        }
        if(!add) sum2+=updates[used/2];
        sum+=add;
        used=0;
    }
    fclose(handlerStream);

    printf("> %ld >> %ld", sum, sum2);

    return 0;
}
