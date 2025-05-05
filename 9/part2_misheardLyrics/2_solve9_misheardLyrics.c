#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>     //malloc
#include <string.h>     //memset

#define endl    printf("\n")

struct llID{
    int id;
    int location;
    struct llID* next;
};

typedef struct llID llID;

void initNode(llID* list, int id, int location){
    list->next = NULL;
    list->id   = id;
    list->location = location;
}

void addEnd(llID* listHead, int id, int location){           //also init the node
    //traverse the singly linked list
    if(listHead->next!=NULL) addEnd((listHead->next), id, location);
    else{
        llID newTail;
        initNode(&newTail, id, location);
        listHead->next = &newTail;
        newTail.next  = NULL;
    }
}

void addHead(llID* listHead, int id, int location){  //switch place of newHead value with current listHead (else the array is invalidated)
     //llID newHead                                  //initialized in local scope, not a passed-in pointer so no "->" 
     /*
     just using "llID newHead" does NOT create a new instance per this function call 
     SO it will be the SAME instance persistant through all of this function call
     WHICH IS NOT WHAT WE WANT
     */
     llID* newHead = malloc(sizeof(struct llID));     //TO THE HEAP + FREE it yourself
     initNode(newHead, listHead->id, listHead->location);
     listHead->id           = id;
     listHead->location     = location;
     if(listHead->next!=NULL) newHead->next = listHead->next;
     else newHead->next = NULL;
     listHead->next = newHead;
}

void decap(llID* list){
    if(list->next!=NULL){
        list->id       = (list->next)->id;
        list->location = (list->next)->location;
        if((list->next)->next!=NULL)
            list->next = (list->next)->next;
        else list->next = NULL;
    }
    else{
        list->id = -1;
        list->next = NULL;
    }
}

int expanded[100000], shrunk[100000], maxID=0, location=0, location2=0, i=0, locTrack=0, loc2Track=0;
unsigned long sum=0;
char val=0;
llID grouping_nums[10000];    //arr [space_filled] = { linkedList(struct(){.ID(sortedMAXMIN), .location}) };
int grouping_dots[100000];         //arr  [location] = space_required; (contains all position)
                                    //best use a linked-list here for appending removing head
                                    //since head adding is done during read-in -> guarranteed that its ALWAYS higher ID in later 
                                    //(just append head and no need to sort)
//for(i in 0 to len(grouping_dots) {get max_id; from dot_location{memset(max_id)}; remove max_id from its linked list; if max_id_space<dot_space_required{grouping_dots[new_location]=remaining_space_required}}

int main(int argc, char* argv[]){

    FILE* stream = fopen(argv[1], "r");

    for(int i=0; i<10000; i++){
        grouping_nums[i].id = -1;       //means this current list is HEADLESS. Add head by initHead the list's first elem
    }

    while(1){
        if((val=fgetc(stream))!=EOF){
            for(i=0; i<(val-48); i++){
                expanded[location+i]=maxID;
                shrunk[location2+i]=maxID;
            }
            if((grouping_nums[val-48]).id!=-1){
                //printf("adding head %d %d %d\n", val-48, maxID, location);
                addHead(&(grouping_nums[val-48]), maxID, location);
            }
            else{
                //printf("init head %d %d %d\n", val-48, maxID, location);
                initNode(&(grouping_nums[val-48]), maxID, location);
            }
            maxID++; location+=(val-48); location2+=(val-48);
        }
        else break;
        if((val=fgetc(stream))!='\n'){
            for(i=0; i<(val-48); i++) expanded[location+i]=-1;
            grouping_dots[location] = val-48;
            location+=(val-48);
        }
        else break;
    }

    loc2Track=location2-1;
    locTrack=location-1;
    /*
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

    /*MISTAKES WERE MADE: this will solve the "fill the space with the highest ID possible" problem, not the actual STATED problem of
     * check from the HIGHEST ID downward if there's space, and if there's none, do NOTHING WITH THAT ID AGAIN
     */
    /*TODO: no longer requires a linked list, just an array for ID lookup with each element contains spans and location*/
    int bidWinner = 0, maxFill = 0;
    for(i=0; i<location; i++){
        if(grouping_dots[i]>0){
            for(int fill=1; fill<=grouping_dots[i]; fill++){
                if(grouping_nums[fill].id > bidWinner){
                    bidWinner = grouping_nums[fill].id;
                    maxFill = fill;
                }
            }
            printf("> requires%d fills%d bidWinner%d loc%d\n", grouping_dots[i], maxFill, bidWinner,(grouping_nums[maxFill].location));
            //fills expanded spaces with grouping_nums[maxFill].id
            //-- memset(&expanded[i], bidWinner, maxFill*(4));          //memset sets BYTES only -> can use to set multiple int
            for(int j=0; j<maxFill; j++) expanded[i + j]=bidWinner;

            //replace (maxFill) elem starting from expanded[grouping_nums[maxFill].location] with -1
            //-- memset(&expanded[grouping_nums[maxFill].location], -1, maxFill*(4));
            for(int j=0; j<maxFill; j++) expanded[(grouping_nums[maxFill].location) + j]=-1;

            //grouping_dots[i+bidWinner] = grouping_dots[i] - bidWinner
            grouping_dots[i+maxFill] = grouping_dots[i] - maxFill;

            //decap grouping_nums[bidWin-1];
            decap(&grouping_nums[maxFill]);
        }
        bidWinner = 0;
        maxFill = 0;
    }

    for(int i=0; i<location; i++){
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
