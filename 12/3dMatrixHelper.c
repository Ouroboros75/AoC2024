#include <stdio.h>
#include <stdlib.h>

int main() {
    char*** perseverance = (char***)malloc(10*sizeof(char*));
    for(int i=0; i<10; i++){
        perseverance[i] = (char**)malloc(10*sizeof(char*));
        for(int j=0; j<10; j++){
            perseverance[i][j] = (char*)malloc(3*sizeof(char*));
            perseverance[i][j][0]=0;
            perseverance[i][j][1]=0;
            perseverance[i][j][2]=0;
        }
    }
    
    perseverance[5][2][0] = 1;
    perseverance[5][2][1] = 2;
    perseverance[5][2][2] = 3;
    
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            for(int k=0; k<3; k++){
                printf("%d", perseverance[i][j][k]);
            }
            printf(" ");
        }
        printf("\n");
    }
    return 0;
}
