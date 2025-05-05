#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define dimension    130
//#define dimension    10
#define endl         printf("\n")
#define show         reveal()
#define u            94
#define r            62
#define d            118
#define l            60

char map[10000][10000] = { 0 };
char pos[10000][10000] = { 0 };
char obs[10000][10000] = { 0 };
char track[10000][10000] = { 0 };        //if obstacle causes movement BACK into same position + same direction -> loop
//requires 4 bits value to track for best mem eff, but since we'll need to do PLENTY of these ops -> trade for performance instead
long queue = 0;                     //bigboy surely would require 128 bit plus variable length
int size = 0;

void reveal(){
    return;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
        }
        endl;
    }
    sleep(1);
    endl;
}

void load(int x, int y){
    queue<<=16;
    queue+=(x<<8) + y;
    //printf("%ld", queue);
    //endl;
}

char check(int x, int y){
    return((abs(x-(char)(queue>>8))==abs((char)(queue>>24)-(char)(queue>>40)) )&&(abs(y-(char)(queue))==abs((char)(queue>>16)-(char)(queue>>32))));
}

void traversal(int x, int y, char override){    //override char to dictate whether to check obstacle placement
    //printf("<%d,%d>", x, y);
    //for(int i=2; i>=0; i--){
    //    printf("(%d,%d) ", (char)(queue>>(8+(i*16)) ), (char)(queue>>(i*16)));
    //}
    //endl;
    switch(map[x][y]){
        case(94):
            for(int i=x-1; i>=0; i--){
                if((map[i][y]!='.')&&(map[i][y]!='X')){
                    //obstacle at next position -> queue up CURRENT position
                    load(i+1,y);
                    map[i+1][y]='>';
                    traversal(i+1, y);
                    return;
                }
                //next position is movable -> check CURRENT position for possible squareloop
                if(check(i+1,y)) obs[i][y]=1;     //place obstacle at the NEXT position
                //count pos
                pos[i+1][y]=1;
                //move up
                map[i+1][y]='X';
                if(i>0){
                    map[i][y]='^';
                    show;
                }
                else{
                    pos[i][y]=1;
                    map[i][y]='X';
                    show;
                    return;
                }
            }
            break;
        case(62):
            for(int j=y+1; j<size; j++){
                if((map[x][j]!='.')&&(map[x][j]!='X')){
                    load(x,j-1);
                    map[x][j-1]='v';
                    traversal(x, j-1);
                    return;
                }
                //next position is movable -> check CURRENT position for possible squareloop
                if(check(x,j-1)) obs[x][j]=1;     //place obstacle at the NEXT position
                //count pos
                pos[x][j-1]=1;
                //move right
                map[x][j-1]='X';

                if(j<size){
                    map[x][j]='>';
                    show;
                }
                else{
                    pos[x][j]=1;
                    map[x][j]='X';
                    show;
                    return;
                }
            }
            break;
        case(118):
            for(int i=x+1; i<size; i++){
                if((map[i][y]!='.')&&(map[i][y]!='X')){
                    load(i-1,y);
                    map[i-1][y]='<';
                    traversal(i-1, y);
                    return;
                }
                //next position is movable -> check CURRENT position for possible squareloop
                if(check(i-1,y)) obs[i][y]=1;     //place obstacle at the NEXT position
                //count pos
                pos[i-1][y]=1;
                //move down
                map[i-1][y]='X';

                if(i!=size-1){
                    map[i][y]='v';
                    show;
                }
                else{
                    pos[i][y]=1;
                    map[i][y]='X';
                    show;
                    return;
                };
            }
            break;
        case(60):
            for(int j=y-1; j>=0; j--){
                //if((x==6)&&(j==4)){
                //    printf("<%d,%d> ", x, j);
                //    for(int i=2; i>=0; i--){
                //        printf("(%d,%d) ", (char)(queue>>(8+(i*16)) ), (char)(queue>>(i*16)));
                //    }
                //    endl;
                //}
                if((map[x][j]!='.')&&(map[x][j]!='X')){
                    load(x,j+1);
                    map[x][j+1]='^';
                    traversal(x, j+1);
                    return;
                }
                //next position is movable -> check CURRENT position for possible squareloop
                if(check(x,j+1)) obs[x][j]=1;     //place obstacle at the NEXT position
                //count pos
                pos[x][j+1]=1;
                //move left
                map[x][j+1]='X';
                if(j>0){
                    map[x][j]='<';
                    show;
                }
                else{
                    pos[x][j]=1;
                    map[x][j]='X';
                    show;
                    return;
                }
            }
            break;
        default:
            puts("-- INPUT ERROR");
            return;
    }
}

int main(int argc, char* argv[]){

    int sum = 0, sum2 = 0;

    FILE* handlerStream = fopen(argv[1], "r");
    for(size=0; fgets(map[size], sizeof(map[0]), handlerStream); size++);
    fclose(handlerStream);

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
        }
        endl;
    }
    endl;
    //do 1 pass, with sum to make sure that its correct + record all traversed positions WITH directions
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if((map[i][j]!='#')&&(map[i][j]!='.')){
                traversal(i, j);
                break;
            }
        }
    }
    //start another pass, but this time test placement of every position in-front and in-trajectory: if in recorded -> loop // break if exited clean
    endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            sum+=pos[i][j];
            printf("%c", map[i][j]);
        }
        endl;
    }
    endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            sum2+=obs[i][j];
            printf("%d", obs[i][j]);
        }
        endl;
    }
    endl;
    printf("> %d >> %d\n", sum, sum2);

    return 0;
}
