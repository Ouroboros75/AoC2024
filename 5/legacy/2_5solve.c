#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define isint(a)   if((a>=48)&&(a<=57))

short arr[10000][10000] = { 0 };
short updates[10000] = { 0 };

int main(int argc, char* argv[]){

    unsigned char readChar, secBreak = 0;
    unsigned int used = 0;
    int readRes = 1, handler = 0, curNum = 0, prevNum = 0, hold = 0;
    long sum = 0;

    handler = open(argv[1], O_RDONLY);              //unsafe input. Returns a FILE DESCRIPTOR
    while(readRes!=0){
        readRes = read(handler, &readChar, 1);
        isint(readChar) curNum = (10*curNum) + readChar - 48;
        else{
            if(readChar=='|'){
                prevNum = curNum;
                secBreak = 0;
            }
            if(readChar=='\n'){
                arr[prevNum][curNum] = 1;
                prevNum = 0;
                if(secBreak==1) break;
                secBreak++;
            }
            curNum = 0;
        }
    }

    for(int i=0; i<100; i++){
        for(int j=0; j<100; j++) if(arr[i][j]==1) printf("%d|%d\n", i, j);

    }

    while(readRes!=0){
        readRes = read(handler, &readChar, 1);
        if(!readRes) break;
        isint(readChar) curNum = (10*curNum) + readChar - 48;
        else{ 
            updates[used++]=curNum;
            if(readChar=='\n'){
                for(int i=0; i<used; i++){
                    for(int j=i+1; j<used; j++){
                        if(arr[updates[j]][updates[i]]!=0){
                            hold = updates[j];
                            updates[j] = updates[i];
                            updates[i] = hold;
                        }
                    }
                    if((i==(used/2))&&(hold!=0)){
                        sum+=updates[i];
                        break;
                    }
                }
                used=0;
                hold=0;
            }
            curNum = 0;
        }
    }
    close(handler);

    printf("> %ld\n", sum);

    return 0;
}
