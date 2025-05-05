#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     //malloc

#define endl    printf("\n")

//precalculated number of stones after 44 pass for well-known markings
unsigned int testable[100000000] = {0};            //reserves 320Mbytes for lookups -> stores up to value 99,999,999. Counts stones after 40 passes

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
            //printf("ooble\n");
            free(localPointer->prev);
        }
        else{
            //printf("ingle\n");
            free(localPointer);
            break;
        }
    }
    //========RESULT (invoker function makes sure input value <= 99,999,999)
    testable[value] = count;
    medium = originalMedium;
    //printf("zany\n");
    return count;
}

int main(int argc, char* argv[]){

    FILE* stream = fopen(argv[1], "r");

    char c;
    unsigned long val, sum = 0;
    int pass = 0, show = 0;
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

    puts("aosidjsaijdsaodjaslkjd");

    arrangement* tail = &head;   //at this point head is tail

    while((c=fgetc(stream))!='\n'){
        ungetc(c, stream);
        fscanf(stream, "%lu", &val);
        tail = addTail(tail, val);
    }

    puts("alkdjsalkdjaskldjqihfrurururuurur");

    arrangement* pointer = &head;
    while(pointer!=NULL){
        printf("%lu-", pointer->value);
        pointer = pointer->next;
    }
    endl;
    printf("-------------------------------------------\n");

    //int boundedPass = (pass <= 35) ? pass : 35;
    for(int i=0; i<pass; i++){
        printf(">pass number %d\n", i);
        pointer = &head;
        while(1){
            if(pointer==NULL) break;
            /*
            if(((i==31)&&(pointer->value==0))||((i==32)&&(pointer->value==1))||((i==33)&&(pointer->value==2024))){
                //printf("cut/");
                sum+=pc;
                cut(pointer);
                pointer = pointer->next;
                continue;
            }
            */
            if((i==35)&&(pointer->value<99999999)){
                if(testable[pointer->value] != 0) sum += testable[pointer->value];
                else sum += personalTransformation(pointer->value);
                cut(pointer);
                pointer = pointer->next;
                free(pointer->prev);                                             //free bird
                continue;
            }
            transformation(pointer);
            if(medium != NULL){
                pointer = medium;
                medium  = NULL;
            }
            else if(pointer->next!=NULL) pointer=pointer->next;
            else break;
        }
        /* show the SET after every transformation */
        if(show>0){
            pointer = &head;
            while(1){
                printf("%lu-", pointer->value);
                if(pointer->next!=NULL) pointer=pointer->next;
                else break;
            }
            endl; endl;
        }
    }

    if(show>0){
        pointer = &head;
        while(1){
            printf("%lu-", pointer->value);
            if(pointer->next!=NULL) pointer=pointer->next;
            else break;
        }
        endl; endl;
    }

    pointer = &head;
    printf("HEADHUNT\n");
    while(1){
        sum++;
        if(pointer->next!=NULL){
            //if(pointer->value < 4294967296) testable[pointer->value]++;
            pointer=pointer->next;
        }
        else break;
    }

    /*
    printf("HEADGAIN\n");
    for(int i=0 ; i < 4294967296; i++){
        printf("- %u\n", i);
        if(testable[i] > occuranceCounter[0]){
            occuranceCounter[0] = i;
            occuranceCounter[1] = occuranceCounter[0];
            occuranceCounter[2] = occuranceCounter[1];
            occuranceCounter[3] = occuranceCounter[2];
            occuranceCounter[4] = occuranceCounter[3];
        }
    }
    */

    printf("BubbaCK\n");
    //printf("> %lu (%d, %d, %d, %d, %d)\n",sum,occuranceCounter[0],occuranceCounter[1],occuranceCounter[2],occuranceCounter[3],occuranceCounter[4]);
    printf(">> %lu\n", sum);

    return 0;
}
