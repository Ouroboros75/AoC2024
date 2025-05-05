#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     //malloc
#include "khash.h"

#define endl    printf("\n")

//precalculated number of stones after 44 pass for well-known markings
unsigned int testable[100000000] = {0};            //reserves 320Mbytes for lookups -> stores up to value 99,999,999. Counts stones after 40 passes
/*TODO:
 * implement a decent hashmap
 * implement DFS (using recursion) instead of BFS right now
 */
//NOTE: scuff polymorphism: gives khash's function and struct definitions, that's why this needs to be invoked outside of main()
KHASH_MAP_INIT_INT64(64, unsigned long*)
khash_t(64) *h;

struct arrangement{
    unsigned long value;
    struct arrangement* next;
    struct arrangement* prev;
};

typedef struct arrangement arrangement;
arrangement *medium = NULL;

arrangement* addTail(arrangement* tail, int value){
    arrangement* newTail = malloc(sizeof(arrangement));
    newTail->value = value;
    newTail->next = NULL;
    newTail->prev = tail;
    tail->next = newTail;
    return newTail;
}

void cut(arrangement* arra){
    //printf("0boog\n");
    if((arra->next!=NULL)&&(arra->prev!=NULL)){
        (arra->prev)->next = arra->next;
        (arra->next)->prev = arra->prev;
    }
    else if(arra->next==NULL) (arra->prev)->next = NULL;
    else if(arra->prev==NULL) (arra->next)->prev = NULL;
    //printf("boogie\n");
}

void split(arrangement* arra, unsigned long value1, unsigned long value2){
    arrangement* newPeerNode = malloc(sizeof(arrangement));
    arra->value        = value1;
    newPeerNode->value = value2;
    newPeerNode->next  = arra->next;
    newPeerNode->prev  = arra;
    if(arra->next!=NULL) arra->next->prev = newPeerNode;
    arra->next         = newPeerNode;
}

//2449752448 wrongly into 18446744071864336768 ANSWER: wrong typecasting in here
void transformation(arrangement* arra){
    if(arra->value==0){
        arra->value=1;
        return;
    }
    unsigned long value = (arra->value), count = 0, divisor = 1;
    while(value>0){
        value = value/10;
        count++;
    }
    if(count%2==0){
        for(int i=1; i<=(count/2); i++) divisor*=10;
        medium = arra->next;
        split(arra, arra->value/(divisor), (unsigned long)((arra->value) - (((unsigned long)(arra->value/divisor))*divisor)));
        return;
    }
    else (arra->value) *= 2024;
}

/* set up newlist for a single head -> transform 40 times -> traverse + add to sum + free list */
unsigned int personalTransformation(int value){
    //--------setup
    int count = 0;
    arrangement* singleHead = malloc(sizeof(arrangement));
    singleHead->value = value;
    singleHead->next = NULL;
    singleHead->prev = NULL;
    //--------transform
    arrangement* localPointer;
    arrangement* originalMedium = medium;
    medium = NULL;

    for(int i=0; i<40; i++){
        localPointer = singleHead;
        while(1){
            if(localPointer==NULL) break;
            transformation(localPointer);
            if(medium != NULL){
                localPointer = medium;
                medium = NULL;
            }
            else if(localPointer->next!=NULL) localPointer = localPointer->next;
            else break;
        }
    }
    //--------traverse+add2sum+freelist
    localPointer = singleHead;
    while(1){
        count++;
        if(localPointer->next!=NULL){
            localPointer=localPointer->next;
            free(localPointer->prev);
        }
        else{
            free(localPointer);
            break;
        }
    }
    //========RESULT (invoker function makes sure input value <= 99,999,999)
    testable[value] = count;
    medium = originalMedium;
    return count;
}

//-----------------------------------------------------------------------------------------------------//

/* neoTransformation:  only provides transformed stones, does NOT modify the linked list */
unsigned long* neoTransformation(unsigned long number){
    //unsigned long purpleRain[3] = {$numberOStones, $stonesNumber1, [$stonesNumber2]};
    //freed when program exits anyway
    unsigned long *purpleRain = malloc(sizeof(unsigned long)*3);
    unsigned long holder = number;
    int count = 0, divisor = 1;
    if(number==0){
        purpleRain[0] = 1; purpleRain[1] = 1;
        return purpleRain;
    }
    while(holder>0){
        holder = holder/10;
        count++;
    }
    if(count%2==0){
        for(int i=1; i<=(count/2); i++) divisor*=10;
        purpleRain[0] = 2; purpleRain[1] = number/divisor;
        purpleRain[2] = (unsigned long)((number) - (((unsigned long)(number/divisor))*divisor));
        return purpleRain;
    }
    else{
        purpleRain[0] = 1; purpleRain[1] = number*2024;
        return purpleRain;
    }
}

//NOTE: BFS with recursion, get as many (element, remainingSteps) into hashmap as possible in order to eliminate excessive branching
//      in theory can also be done using an explicit stack implementation
unsigned long recurve(unsigned long number, int remainingSteps){
    //if (number, remainingSteps) in hashmap -> returns hashmap[(number, remainingSteps)]
    int ret, is_missing;
    unsigned long holdder = 0;
    khiter_t k = kh_get(64, h, number);

    if(kh_exist(h, k)){  //h must not be empty (containing no bucket with data) when this is invoked)
        if(kh_value(h, k)[remainingSteps]!=0) return kh_value(h, k)[remainingSteps];
    }
    else{
        k = kh_put(64, h, number, &ret);
        //kh_value(h, k) = malloc(sizeof(unsigned long)*75);    //malloc does NOT initialize data to '0'
        kh_value(h, k) = calloc(75, sizeof(unsigned long));
    }

    unsigned long *transformedStones = neoTransformation(number); //go out of scope at the end of this function

    for(int i=1; i<=(transformedStones[0]); i++){
        printf("- %lu at %d steps remain \n", transformedStones[i], remainingSteps);
        //NOTE: cannot do this since after recursion: "k", as an iterator, is invalidated 
        //kh_value(h, k)[remainingSteps] += ((remainingSteps!=1) ? recurve(transformedStones[i], remainingSteps-1) : transformedStones[0]);
        holdder = ((remainingSteps!=0) ? recurve(transformedStones[i], remainingSteps-1) : 1);
        k = kh_get(64, h, number);
        kh_value(h, k)[remainingSteps] += holdder;
        if(remainingSteps==0) printf("+++ adding %lu for number %lu\n", kh_value(h, k)[remainingSteps], number);
    }
    return(kh_value(h, k)[remainingSteps]);
}

//-----------------------------------------------------------------------------------------------------//

int main(int argc, char* argv[]){

    FILE* stream = fopen(argv[1], "r");

    char c;
    unsigned long val, sum = 0;
    int pass = 0, show = 0;
    int ret, is_missing;
    khiter_t k;
    h = kh_init(64);
    k = kh_put(64, h, -1, &ret);

    for(int i=0; (char)(argv[2][i])!=0; i++) pass = 10*pass + (char)(argv[2][i]) - 48;
    printf("pass==%d\n", pass);
    for(int i=0; (char)(argv[3][i])!=0; i++) show = 10*show + (char)(argv[3][i]) - 48;
    printf("show==%d\n", show);

    arrangement head;
    //arrangement* tail = malloc(sizeof(arrangement));

    fscanf(stream, "%lu", &val);
    head.value = val;
    head.next = NULL;
    head.prev = NULL;

    arrangement* tail = &head;   //at this point head is tail

    while((c=fgetc(stream))!='\n'){
        ungetc(c, stream);
        fscanf(stream, "%lu", &val);
        tail = addTail(tail, val);
    }

    arrangement* pointer = &head;
    while(pointer!=NULL){
        printf("%lu-", pointer->value);
        pointer = pointer->next;
    }
    endl;

    printf("------------------Read whole inpuT-----------------\n");

    pointer = &head;
    printf("HEADHUNT\n");
    while(1){
        puts("great Sloth");
        sum += recurve(pointer->value, pass-1);
        if(pointer->next!=NULL) pointer=pointer->next;
        else break;
    }

    printf(">> %lu\n", sum);

    return 0;
}
