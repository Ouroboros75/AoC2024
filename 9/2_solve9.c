#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>     //malloc
#include <string.h>     //memset

#define endl    printf("\n")

typedef struct llID{
    int location;
    int span;
}llID;

int expanded[100000], shrunk[100000], maxID=0, location=0, location2=0, i=0, locTrack=0, loc2Track=0;
unsigned long sum=0;
char val=0;

llID grouping_nums[10000];
int grouping_dots[100000];

int main(int argc, char* argv[]){

    FILE* stream = fopen(argv[1], "r");

    for(int i=0; i<10000; i++) grouping_nums[i].location = -1;

    while(1){
        if((val=fgetc(stream))!=EOF){
            for(i=0; i<(val-48); i++){
                expanded[location+i]=maxID;
                shrunk[location2+i]=maxID;
            }
            grouping_nums[maxID].location = location;
            grouping_nums[maxID].span     = val-48;
            maxID++; location+=(val-48); location2+=(val-48);
        }
        else break;
        if((val=fgetc(stream))!='\n'){
            for(i=0; i<(val-48); i++) expanded[location+i]=-1;
            grouping_dots[location] = val-48;
            location += (val-48);
        }
        else break;
    }

    /* disp
    loc2Track=location2-1;
    locTrack=location-1;
    llID pointer;
    printf("--------------------------------------------\n");
    for(i=0; i<10; i++){
        pointer = grouping_nums[i];
        printf("fills %d: ", i);
        while(1){
            printf("(Id=%d location=%d) ", pointer.id, pointer.location);
            if(pointer.next!=NULL) pointer = *(pointer.next);
            else break;
        }
        endl;
    }
    for(i=0; i<location; i++){
        printf("-%d", grouping_dots[i]);
    }
    endl;
    printf("--------------------------------------------\n");
    */

    /* -+part1+-
    for(i=0; i<location; i++){
        if(expanded[i]==-1){
            expanded[i]=shrunk[loc2Track];
            loc2Track--;
            if(loc2Track<0) break;
        }
    }

    for(i=location-1; i>=location2; i--) expanded[i]=-1;

    for(i=0; i<location; i++){
        if(expanded[i]!=-1){
            sum += i*expanded[i];
        }
        pointer = grouping_nums[grouping_dots[i]];
        printf("fills %d: ", grouping_dots[i]);
        while(1){
            printf("(Id=%d location=%d) ", pointer.id, pointer.location);
            if(pointer.next!=NULL) pointer = *(pointer.next);
            else break;
        }
        //if(grouping_dots[i]==3) decap(&grouping_nums[grouping_dots[i]]);
        endl;
    }
    endl;
    */

    //~1 second with print, else ~0.9 second
    //no printing with O3 native -> ~0.3 second
    for(i=maxID; i>=0; i--){
        for(int j=0; j<location; j++){
            if((grouping_dots[j] >= grouping_nums[i].span)&&(j < grouping_nums[i].location)){   //j==loc, grouping_dots[j]==freespace
                for(int fill=0; fill<grouping_nums[i].span; fill++){
                    expanded[j + fill] = i;
                    expanded[grouping_nums[i].location + fill] = -1;
                }
                grouping_dots[j + grouping_nums[i].span] += grouping_dots[j] - grouping_nums[i].span;
                grouping_dots[j]=0;
                break;
            }
        }
    }

    for(i=0; i<location; i++){
        if(expanded[i]!=-1){
            //printf("%d", expanded[i]);
            sum += i*expanded[i];
        }
        //else printf(".");
    }
    endl;

    printf("> %lu", sum);
    endl;

    return 0;
}
