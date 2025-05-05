#include <unistd.h>
#include <stdio.h>
#include <string.h> //memcpy
#include <stdlib.h> //malloc

#define endl         printf("\n")

char **pathfinder;
unsigned long sum = 0;
int used;

void recurve(int x, int y){
    if(x-1>=0) if(pathfinder[x-1][y]-pathfinder[x][y]==1){
        if(pathfinder[x-1][y]==9) sum++;
        else recurve(x-1, y);
    }
    if(x+1<used) if(pathfinder[x+1][y]-pathfinder[x][y]==1){
        if(pathfinder[x+1][y]==9) sum++;
        else recurve(x+1, y);
    }
    if(y-1>=0) if(pathfinder[x][y-1]-pathfinder[x][y]==1){
        if(pathfinder[x][y-1]==9) sum++;
        else recurve(x, y-1);
    }
    if(y+1<used) if(pathfinder[x][y+1]-pathfinder[x][y]==1){
        if(pathfinder[x][y+1]==9) sum++;
        else recurve(x, y+1);
    }
}

int main(int argc, char* argv[]){

    FILE* handlerStream = fopen(argv[1], "r");
    char demo[10000], hold;
    fgets(demo, 10000, handlerStream);      //using fgetc would require many scalls
    for(used=0; demo[used]!='\n'; used++);
   
    //https://c-faq.com/aryptr/dynmuldimary.html
	pathfinder = malloc(used * sizeof(char *));
	for(int i = 0; i < used; i++)
		pathfinder[i] = malloc(used * sizeof(char));         //assuming always square matrix

    memcpy(pathfinder[0], demo, used);
    for(int i = 1; i < used; i++){
        fgets(pathfinder[i], used+1, handlerStream);
        fgetc(handlerStream);   //get \n
    }

    for(int i=0; i<used; i++){
        for(int j=0; j<used; j++){
            pathfinder[i][j] -= 48;
            //printf("%d", pathfinder[i][j]);
        }
        //endl;
    }

    for(int i=0; i<used; i++){
        for(int j=0; j<used; j++){
            if(pathfinder[i][j]==0){
                recurve(i, j);
            }
        }
    }

    printf("> %ld\n", sum);

    return 0;
}
