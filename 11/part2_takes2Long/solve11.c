#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>     //malloc

#define endl    printf("\n")

//precalculated number of stones after 44 pass for well-known markings
//int pc[3] = { 108093610, 54046805, 1916299 };
int pc = 54046805;      //mark 0 at pass 75-44==31 / mark 1 pass 75-43==32 / mark 2024 pass 75-42==33
int pcTest = 7;         //mark 2024 at pass 7-5=2
char testable[4294967296] = {0};
unsigned int occuranceCounter[5];       //count 5 most occurred int sized element at 40 passes

struct arrangement{
    unsigned long value;
    struct arrangement* next;
    struct arrangement* prev;
};

typedef struct arrangement arrangement;
arrangement *medium = 1;

arrangement* addTail(arrangement* tail, int value){
    arrangement* newTail = malloc(sizeof(arrangement));
    newTail->value = value;
    newTail->prev = tail;
    tail->next = newTail;
    return newTail;
}

void cut(arrangement* arra){
    if((arra->next!=NULL)&&(arra->prev!=NULL)){
        (arra->prev)->next = arra->next;
        (arra->next)->prev = arra->prev;
    }
    else if(arra->next==NULL) (arra->prev)->next = NULL;
    else if(arra->prev==NULL) (arra->next)->prev = NULL;
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
        //printf(">%d", ((arra->value) - (((int)(arra->value/divisor))*divisor)));
        split(arra, arra->value/(divisor), (unsigned long)((arra->value) - (((unsigned long)(arra->value/divisor))*divisor)));
        return;
    }
    else (arra->value) *= 2024;
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
    head.prev = NULL;
    tail->next = NULL;
    tail->prev = &head;

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
            /*
            if(((i==31)&&(pointer->value==0))||((i==32)&&(pointer->value==1))||((i==33)&&(pointer->value==2024))){
                //printf("cut/");
                sum+=pc;
                cut(pointer);
                pointer = pointer->next;
                continue;
            }
            */
            /*
            if((i==2)&&(pointer->value==2024)){
                printf("cut/");
                sum+=pcTest;
                cut(pointer);
                pointer = pointer->next;
                continue;
            }
            */
            transformation(pointer);
            if(medium != 1){
                pointer = medium;
                medium  = 1;
            }
            else if(pointer->next!=NULL) pointer=pointer->next;
            else break;
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
            if(pointer->value < 4294967296){
                testable[pointer->value]++;
            }
            pointer=pointer->next;
        }
        else break;
    }

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

    printf("BubbaCK\n");
    printf("> %lu (%d, %d, %d, %d, %d)\n",sum, occuranceCounter[0], occuranceCounter[1], occuranceCounter[2], occuranceCounter[3], occuranceCounter[4]);

    return 0;
}
