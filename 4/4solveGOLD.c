#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define r   140
//#define c   140
#define r   15000
#define c   15000

char mat[r+6][c+6]={0};  //(initialized with value -> known at compile time && outside of any function) ==> go on the initializedDataSegment

int main(int argc, char* argv[]){

    unsigned char readChar;
    int readRes = 1, sum = 0, handler = 0, sum2 = 0;
    FILE* handler_stream;
    //char *mat = (char *)malloc((r+6) * (c+6) * sizeof(char));
    int mask[4]={0}, face=0;
    mask[0] = (('M'<<24) + ('M'<<16) + ('S'<<8) + 'S');
    mask[1] = (('M'<<24) + ('S'<<16) + ('M'<<8) + 'S'); 
    mask[2] = (('S'<<24) + ('M'<<16) + ('S'<<8) + 'M'); 
    mask[3] = (('S'<<24) + ('S'<<16) + ('M'<<8) + 'M');

    /* AD-HOC, OLD AND BUSTED input file parsing */
    /*
    handler = open(argv[1], O_RDONLY);              //unsafe input, returns a FILE DESCRIPTOR
    for(int i=3; i<r+3; i++){                   //1 read per char for (15000*15000) char => about 22->30 seconds of sys time....
        for(int j=3; j<c+3; j++){
            readRes = read(handler, &(mat[i][j]), 1);
        }
        readRes = read(handler, &readChar, 1);
    }
    close(handler);
    */
    
    /* PROPER, NEW HOTNESS input file parsing */
    handler_stream = fopen(argv[1], "r");       //unsafe input, returns a FILE* STREAM
    for(int w=0; fgets(mat[w], sizeof(mat[0]), handler_stream); w++);    //0.3 seconds sys time!!!.. BUT, is it safe??
    fclose(handler_stream);
    
    //could rotate the matrix by 2 elems to 1 side 8 times and only parse forward instead
    for(int i=3; i<r+3; i++){
        for(int j=3; j<c+3; j++){
            if(mat[i][j]=='X'){
                for(int k=-1; k<2; k++){
                    if(((mat[i+1][j+(1*k)])=='M')&&((mat[i+2][j+(2*k)])=='A')&&((mat[i+3][j+(3*k)])=='S')) sum++;
                    if(((mat[i-1][j+(1*k)])=='M')&&((mat[i-2][j+(2*k)])=='A')&&((mat[i-3][j+(3*k)])=='S')) sum++;
                    if(k!=0) if(((mat[i][j+(1*k)])=='M')&&((mat[i][j+(2*k)])=='A')&&((mat[i][j+(3*k)])=='S')) sum++;
                }
            }
            if(mat[i][j]=='A'){
                face = (mat[i-1][j-1] << 24) + (mat[i+1][j-1] << 16) + (mat[i-1][j+1] << 8) +(mat[i+1][j+1]);
                for(int i=0; i<4; i++){
                    if(face==mask[i]){
                        sum2++;
                        break;
                    }
                }
            }
        }
    }

    //FORMAT-STRING-VULNERABILITY:
    /* If you use printf to output user input value then
     * pretty much just make sure you use printf with additional argument and not just format argument
     * which leads to undefined behavior (from what i've seen pops and output a stack value)
     * ALSO user can pass a format as the argument to FORMAT the popped and printed stack value -> LEAKS INFO
     */
    printf(">  %d >> %d\n", sum, sum2);
    return 0;
}
