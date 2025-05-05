#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define r   140
#define c   140
//#define r   10
//#define c   10

#define endl     printf("\n")

int main(int argc, char* argv[]){

    unsigned char readChar;
    int readRes = 1, sum = 0, handler =0;
    char mat[r+6][c+6]={0};

    handler = open(argv[1], O_RDONLY);      //unsafe input

    for(int i=3; i<r+3; i++){
        for(int j=3; j<c+3; j++){
            readRes = read(handler, &(mat[i][j]), 1);
        }
        readRes = read(handler, &readChar, 1);  //endl
    }

    for(int i=3; i<r+3; i++){
        for(int j=3; j<c+3; j++){
            printf("%c", mat[i][j]);
        }
        endl;
    }

    //dishonorable, should have tried to rotate the matrix
    for(int i=3; i<r+3; i++){
        for(int j=3; j<c+3; j++){
            if(mat[i][j]=='X'){
                for(int k=-1; k<2; k++){
                    if(((mat[i+1][j+(1*k)])=='M')&&((mat[i+2][j+(2*k)])=='A')&&((mat[i+3][j+(3*k)])=='S')){
                        printf("+ (%d,%d)\n", i-3, j-3);
                        sum+=1;
                    }
                    if(((mat[i-1][j+(1*k)])=='M')&&((mat[i-2][j+(2*k)])=='A')&&((mat[i-3][j+(3*k)])=='S')){
                        printf("+ (%d,%d)\n", i-3, j-3);
                        sum+=1;
                    }
                    if(((mat[i][j+(1*k)])=='M')&&((mat[i][j+(2*k)])=='A')&&((mat[i][j+(3*k)])=='S')){
                        printf("+ (%d,%d)\n", i-3, j-3);
                        sum+=1;
                    }
                }
            }
        }
    }

    printf("> %d\n", sum);

    return 0;
}
