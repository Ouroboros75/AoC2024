#include <unistd.h>
#include <stdio.h>

#define endl         printf("\n")

char map[150][150] = { 0 };
char pos[150][150] = { 0 };
char pos2[150][150] = { 0 };
int size = 0, sum = 0, sum2 = 0;
char xc0 = 0, yc0 = 0, xc1 = 0, yc1 = 0;
char xp0 = 0, yp0 = 0, xp1 =0, yp1 = 0;

typedef struct freq_l{
    short coord[150];
    short used;
} freq_l;

freq_l freq[255];

void append(char character, short loc){
    freq[character].coord[freq[character].used++] = loc;
}

int main(int argc, char* argv[]){

    FILE* handlerStream = fopen(argv[1], "r");
    for(size=0; fgets(map[size], sizeof(map[0]), handlerStream); size++);
    fclose(handlerStream);

    for(short i=0; i<size; i++){
        for(short j=0; j<size; j++){                             //assuming square
            if(map[i][j]!=46) append(map[i][j], (i<<8)+j);
        }
    }

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%c", map[i][j]);
        }
        endl;
    }
    endl;

    for(int c=0; c<255; c++){                                  //All possible ASCII character
        if(freq[c].used>0){                                     //check if this character appears in grid
           for(int l=0; l<(freq[c].used); l++){
               for(int ll=l+1; ll<(freq[c].used); ll++){
                   xc0 = (char)(freq[c].coord[l] >> 8);
                   yc0 = (char)(freq[c].coord[l]);
                   xc1 = (char)(freq[c].coord[ll] >> 8);
                   yc1 = (char)(freq[c].coord[ll]);
                   pos2[xc0][yc0] = 1; pos2[xc1][yc1] = 1;
                   xp0 = xc0 + (xc0-xc1); yp0 = yc0 + (yc0-yc1);
                   xp1 = xc1 + (xc1-xc0); yp1 = yc1 + (yc1-yc0);
                   if((xp0>=0)&&(xp0<size)&&(yp0>=0)&&(yp0<size)){
                       pos[xp0][yp0] = 1; 
                       while((xp0>=0)&&(xp0<size)&&(yp0>=0)&&(yp0<size)){
                            pos2[xp0][yp0] = 1;
                            xp0 = xp0 + (xc0-xc1); yp0 = yp0 + (yc0-yc1);
                       }
                   }
                   if((xp1>=0)&&(xp1<size)&&(yp1>=0)&&(yp1<size)){
                       pos[xp1][yp1] = 1;
                       while((xp1>=0)&&(xp1<size)&&(yp1>=0)&&(yp1<size)){
                            pos2[xp1][yp1] = 1;
                            xp1 = xp1 + (xc1-xc0); yp1 = yp1 + (yc1-yc0);
                       }
                   }
               }
           }
        }
    }

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(pos[i][j]==1){
                sum++;
                printf("#");
            }
            else printf("%c", map[i][j]);
        }
        endl;
    }
    endl;

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(pos2[i][j]==1){
                sum2++;
                printf("#");
            }
            else printf("%c", map[i][j]);
        }
        endl;
    }
    endl;

    printf("> %d >> %d\n", sum, sum2);

    return 0;
}
