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

char map[10000][10000] = { 0 };
char pos[10000][10000] = { 0 };
int size = 0;

void reveal(){
    return;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
        }
        endl;
    }
    sleep(0.9);
    endl;
}

void traversal(int x, int y){
    switch(map[x][y]){
        case(94):
            for(int i=x-1; i>=0; i--){
                if((map[i][y]!='.')&&(map[i][y]!='X')){
                    printf("correct");
                    map[i+1][y]='>';
                    traversal(i+1, y);
                    return;
                }
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
                printf("%d\n",j);
                if((map[x][j]!='.')&&(map[x][j]!='X')){
                    map[x][j-1]='v';
                    traversal(x, j-1);
                    return;
                }
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
                    map[i-1][y]='<';
                    traversal(i-1, y);
                    return;
                }
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
                if((map[x][j]!='.')&&(map[x][j]!='X')){
                    map[x][j+1]='^';
                    traversal(x, j+1);
                    return;
                }
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
            printf("-- INPUT ERROR");
            endl;
            return;
    }
}

int main(int argc, char* argv[]){

    int sum = 0;

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
    endl;

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if((map[i][j]!='#')&&(map[i][j]!='.')){
                traversal(i, j);
                break;
            }
        }
    }

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            sum+=pos[i][j];
            printf("%c", map[i][j]);
        }
        endl;
    }

    printf("> %d\n", sum);

    return 0;
}
