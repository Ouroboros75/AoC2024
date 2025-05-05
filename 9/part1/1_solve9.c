#include <unistd.h>
#include <stdio.h>

#define endl    printf("\n")

int main(int argc, char* argv[]){

    FILE* stream = fopen(argv[1], "r");
    int expanded[100000], shrunk[100000], maxID=0, location=0, location2=0, i=0, locTrack=0, loc2Track=0;
    unsigned long sum=0;
    char val=0;

    struct llID{
        int id;
        int location;
        struct llID* prev;
        struct llID* next;
    };

    typedef struct llID llID;

    void initNode(llID* list, int id, int location){
        list->prev = NULL;
        list->next = NULL;
        list->id   = id;
        list->location = location;
    }

    void addNode(llID* listHead, llID* newNode, int id, int location){
        if(listHead->next!=NULL) addNode((listHead->next), newNode, id, location);
        else{
            //llID new_node;
            //initNode(new_node, id, location);
            listHead->next = newNode;
            newNode->prev  = listHead;
        }
    }

    void decap(llID* list){
        list->id       = (list->next)->id;
        list->location = (list->next)->location;
        list->next     = (list->next)->next;
    }

    llID grouping_nums[10000];    //arr [space_filled] = { linkedList(struct(){.ID(sortedMAXMIN), .location}) };
    short grouping_dots[10000];         //arr  [location] = space_required;
                                        //best use a linked-list here for appending removing head
                                        //since head adding is done during read-in -> guarranteed that its ALWAYS higher ID in later 
                                        //(just append head and no need to sort)
    //for(i in 0 to dot_space_required) {get max_id; from dot_location{memset(max_id)}; remove max_id from its linked list; if max_id_space<dot_space_required{grouping_dots[new_location]=remaining_space_required}}

    while(1){
        if((val=fgetc(stream))!=EOF){
            for(i=0; i<(val-48); i++){
                expanded[location+i]=maxID;
                shrunk[location2+i]=maxID;
            }
            maxID++; location+=(val-48); location2+=(val-48);
            printf("%d\n", location);
        }
        else break;
        if((val=fgetc(stream))!='\n'){
            for(i=0; i<(val-48); i++) expanded[location+i]=-1;
            location+=(val-48);
            printf("%d\n", location);
        }
        else break;
    }

    for(i=0; i<location; i++) printf("%d", expanded[i]);
    endl;
    for(i=0; i<location2; i++) printf("%d", shrunk[i]);
    endl;

    loc2Track=location2-1;
    locTrack=location-1;

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
            printf("%d", expanded[i]);
            sum += i*expanded[i];
        }
        else printf(".");
    }
    endl;

    printf("> %lu", sum);
    endl;

    return 0;
}
