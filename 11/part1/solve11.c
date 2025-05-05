#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     //malloc
#include <math.h>

#define endl    printf("\n")

struct arrangement{
    unsigned long value;
    struct arrangement* next;
};

typedef struct arrangement arrangement;
arrangement *medium = 1;

arrangement* addTail(arrangement* tail, int value){
    arrangement* newTail = malloc(sizeof(arrangement));
    newTail->value = value;
    tail->next = newTail;
    return newTail;
}

void split(arrangement* arra, unsigned long value1, unsigned long value2){
    arrangement* newPeerNode = malloc(sizeof(arrangement));
    arra->value        = value1;
    newPeerNode->value = value2;
    newPeerNode->next  = arra->next;
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
        //printf(">%d", ((arra->value) - (((int)(arra->value/divisor))*divisor)));
        split(arra, arra->value/(divisor), (unsigned long)((arra->value) - (((unsigned long)(arra->value/divisor))*divisor)));
        return;
    }
    else{
        (arra->value) *= 2024;
    }
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
    arrangement* tail = malloc(sizeof(arrangement));

    //assuming there must be at least 2 valid integer in input
    fscanf(stream, "%lu", &val);
    head.value = val;           //denotes head of list
    fgetc(stream);                  //get 1 space
    fscanf(stream, "%lu", &val);
    tail->value = val;

    head.next = tail;
    tail->next = NULL;

    while((c=fgetc(stream))!='\n'){
        ungetc(c, stream);
        fscanf(stream, "%lu", &val);
        tail = addTail(tail, val);
    }

    arrangement* pointer = &head;
    while(1){
        printf("%lu-", pointer->value);
        if(pointer->next!=NULL) pointer=pointer->next;
        else break;
    }
    endl;
    printf("-------------------------------------------\n");

    for(int i=0; i<pass; i++){
        printf(">pass number %d\n", i);
        pointer = &head;
        while(1){
            if(pointer==NULL) break;
            transformation(pointer);
            if(medium != 1){
                pointer = medium;
                medium = 1;
            }
            else if(pointer->next!=NULL) pointer=pointer->next;
            else break;
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
    while(1){
        sum++;
        if(pointer->next!=NULL) pointer=pointer->next;
        else break;
    }

    printf("> %lu\n", sum);

    return 0;
}
