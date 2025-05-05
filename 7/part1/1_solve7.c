#include <unistd.h>
#include <stdio.h>

#define endl    printf("\n")

int main(int argc, char* argv[]){
    char c;
    unsigned long test = 0, sum = 0, hold = 0;
    FILE* stream = fopen(argv[1], "r");
    unsigned int nums[20] = {0}, ops[19] = {0}, used = 0, shuffle = 0;
    while(fscanf(stream, "%ld:", &test)!=EOF){
        for(used=0; fgetc(stream)==' '; used++){
            fscanf(stream, "%d", &nums[used]);
        }
        //-----power cal
        //-----shuffle = (2^(used-1))-1;
        shuffle=2;
        for(int l=0; l<used-1; l++){
            shuffle *= 2;
        }
        shuffle--;
        //-----disp
        printf("<%d/%d> %ld:", used, shuffle, test);
        for(int i=0; i<used; i++) printf(" %d", nums[i]);
        endl;
        //-----calc
        for(int j=0; j<=shuffle; j++){
            for(int k=1; k<=used; k++){
                hold = (((j>>(used - k))&0x00000001) > 0) ? (hold * nums[k-1]) : (hold + nums[k-1]);
            }
            if(hold == test){
                sum += test;
                break;
            }
            hold = 0;
        }
        hold = 0;
        used = 0;
    }

    printf("> %ld", sum);
    endl;

    fclose(stream);
    return 0;
}
