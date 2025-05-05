#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define dimension    130
//#define dimension    10
#define endl         printf("\n")

#define disp
#ifdef disp
#define show if(override>0) reveal()
#else
#undef show
#define show do {} while(0)
#endif

char map[150][150] = { 0 }, count = 0;
char pos[150][150] = { 0 };
char obs[150][150] = { 0 };
char track[150][150][4] = { 0 };        //if obstacle causes movement BACK into same position + same direction -> loop
//requires 4 bits value to track for best mem eff, but since we'll need to do PLENTY of these ops -> trade for performance instead
long queue = 0;                     //bigboy surely would require 128 bit plus variable length
int queueDir = 0, size = 0;
int nx=0, ny=0, nd = 0, ox = 0, oy = 0;
char od = 0;
int ops = 0;

void reveal(){
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
            //if(map[i][j]=='X') map[i][j]='.';
        }
        endl;
    }
    usleep(500000);
    //sleep(1);
    endl;
}

/*
void load(char x, char y, char d){
    queue<<=16;
    queue+=(nx<<8) + ny;
    queueDir<<=8;
    queueDir+=nd;
    nx = x; ny = y; nd = d;
}
*/

/*
char check(char x, char y, char d){
    //return((abs(x-(char)(queue>>8))==abs((char)(queue>>24)-(char)(queue>>40)) )&&(abs(y-(char)\
    (queue))==abs((char)(queue>>16)-(char)(queue>>32)))&&(d==(queueDir>>24)));
    //return((nx==(char)(queue>>40))&&(ny==(char)(queue>>32))&&(nd==(char)(queueDir>>24)));
    //printf("(%d==%d)&&(%d==%d)\n", nx, (char)(queue>>56), ny, (char)(queue>>48));
    return((nx==(char)(queue>>56))&&(ny==(char)(queue>>48))&&(nd==(char)(queueDir>>24)));
}
*/

char traversal(int x, int y, char override, char specOps){    
    switch(map[x][y]){
        case(94):        //UP
            for(int i=x-1; i>=0; i--){
                if(override>1){
                    if(track[i+1][y][0]==0) track[i+1][y][0]=1;
                    else{
                        map[i+1][y]='.';
                        return 0;
                    }
                }
                if(map[i][y]!='.'){
                    map[i+1][y]='>';
                    return(traversal(i+1, y, override, specOps));
                }
                if(override==1){
                    pos[i+1][y]=1;
                    map[i][y]='O';
                    map[ox][oy] = od;
                    if(traversal(ox, oy, 2, ops)==0){ 
                        obs[i][y]=1;
                    }
                    for(int l=0; l<150; l++) for(int k=0; k<150; k++) memset(track[l][k], 0, 4);
                    map[i][y]='.';
                }
                map[i+1][y] = '.';
                if(i!=0) map[i][y]='^';
                else{
                    if(override==1) pos[i][y]=1;
                    return 1;
                }
            }
            break;
        case(62):           //RIGHT;
            for(int j=y+1; j<size; j++){
                if(override>1){
                    if(track[x][j-1][1]==0) track[x][j-1][1]=1;
                    else{
                        map[x][j-1]='.';
                        return 0;
                    }
                }
                if(map[x][j]!='.'){
                    map[x][j-1]='v';
                    return(traversal(x, j-1, override, specOps));
                }
                if(override==1){
                    pos[x][j-1]=1;
                    map[x][j]='O';
                    map[ox][oy] = od;
                    if(traversal(ox, oy, 2, ops)==0){ 
                        obs[x][j]=1;
                    }
                    for(int l=0; l<150; l++) for(int k=0; k<150; k++) memset(track[l][k], 0, 4);
                    map[x][j]='.';
                }
                map[x][j-1] = '.';
                if(j!=size-1) map[x][j]='>';
                else{
                    if(override==1) pos[x][j]=1;
                    return 1;
                }
            }
            break;
        case(118):          //DOWN
            for(int i=x+1; i<size; i++){
                if(override>1){
                    if(track[i-1][y][2]==0) track[i-1][y][2]=1;
                    else{
                        map[i-1][y]='.';
                        return 0;
                    }
                }
                if(map[i][y]!='.'){
                    map[i-1][y]='<';
                    return(traversal(i-1, y, override, specOps));
                }
                if(override==1){
                    pos[i-1][y]=1;
                    map[i][y]='O';
                    if(traversal(ox, oy, 2, ops)==0){ 
                        obs[i][y]=1;
                    }
                    for(int l=0; l<150; l++) for(int k=0; k<150; k++) memset(track[l][k], 0, 4);
                    map[i][y]='.';
                }
                map[i-1][y] = '.';
                if(i!=size-1) map[i][y]='v';
                else{
                    if(override==1) pos[i][y]=1;
                    return 1;
                }
            }
            break;
        case(60):           //LEFT
            for(int j=y-1; j>=0; j--){
                if(override>1){
                    if(track[x][j+1][3]==0) track[x][j+1][3]=1;
                    else{
                        map[x][j+1]='.';
                        return 0;
                    }
                }
                if(map[x][j]!='.'){
                    map[x][j+1]='^';
                    return(traversal(x, j+1, override, specOps));
                }
                if(override==1){
                    pos[x][j+1]=1;
                    map[x][j]='O';
                    map[ox][oy] = od;
                    if(traversal(ox, oy, 2, ops)==0){ 
                        obs[x][j]=1;
                    }
                    for(int l=0; l<150; l++) for(int k=0; k<150; k++) memset(track[l][k], 0, 4);
                    map[x][j]='.';
                }
                map[x][j+1] = '.';
                if(j!=0) map[x][j]='<';          
                else{
                    if(override==1) pos[x][j]=1;
                    return 1;
                }
            }
            break;
        default:
            puts("-- INPUT ERROR");
            return -1;
    }
}

int main(int argc, char* argv[]){

    int sum = 0;
    int sum2 = 0;

    FILE* handlerStream = fopen(argv[1], "r");
    for(size=0; fgets(map[size], sizeof(map[0]), handlerStream); size++);
    fclose(handlerStream);

    /*
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
        }
        endl;
    }
    endl;
    */
    //do 1 pass, with sum to make sure that its correct + record all traversed positions WITH directions
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if((map[i][j]!='#')&&(map[i][j]!='.')){
                ox = i;
                oy = j;
                od = map[i][j];
                traversal(i, j, 1, 0);
                break;
            }
        }
    }
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            sum+=pos[i][j];
            //printf("%d", pos[i][j]);
        }
        endl;
    }
    printf("> %d\n", sum);
    ////start another pass, but this time test placement of every position in-front and in-trajectory: if in recorded -> loop // break if exited clean
    //map[ox][oy] = od;
    //traversal(ox, oy, 1);
    //
    /* MISTAKE:
     * Correctly (maybe, still) reasoned that you only have to test obstacle placement along the path of travel
     * BUT, incorrectly assumed that you only have to test the PREVIOUS position
     * WHILE you actually have to go from the BEGINNING, since with the new obstacle placement, cursor may not have been able to reach
     * the PREVIOUS position at all (exited beforehand)
     */
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(obs[i][j]){
                sum2+=obs[i][j];
                printf("O");
            }
            else printf("%c", map[i][j]);
        }
        endl;
    }
    printf(">> %d\n", sum2);
    printf("-- %d\n", size);

    return 0;
}
