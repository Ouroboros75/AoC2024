#include <unistd.h>
#include <stdio.h>

#define endl    printf("\n")

int main(int argc, char* argv[]){
    char c, breakout = 0;
    unsigned long test = 0, sum = 0, hold = 0;
    FILE* stream = fopen(argv[1], "r");
    unsigned int nums[20] = {0}, ops[19] = {0}, used = 0, shuffle = 0, digits = 0, inter = 0;
    unsigned char mask1[19] = {0}, mask2[19] = {0};
    while(fscanf(stream, "%ld:", &test)!=EOF){
        for(used=0; fgetc(stream)==' '; used++){
            fscanf(stream, "%d", &nums[used]);
        }
        //-----power cal
        //-----shuffle = (2^(used-1))-1;
        shuffle=2;
        for(int l=0; l<used-1; l++) shuffle *= 2;
        shuffle--;
        //-----disp
        printf("<%d/%d> %ld:", used, shuffle, test);
        for(int i=0; i<used; i++) printf(" %d", nums[i]);
        endl;
        //-----calc
        /*
        for(int m=0; m<=used; m++){
            for(int j=0; j<=shuffle; j++){
                for(int k=1; k<=used; k++){
                    if(k==m){
                        inter = nums[k-1];
                        while(inter>0){
                            digits++;
                            inter=inter/10;
                        }
                        for(int n=0; n<digits; n++) hold*=10; 
                        hold += nums[k-1];
                        digits=0;
                        inter=0;
                    }
                    else hold = (((j>>(used - k))&0x00000001) > 0) ? (hold * nums[k-1]) : (hold + nums[k-1]);
                }
                if(hold == test){
                    sum += test;
                    breakout = 1;
                    break;
                }
                hold = 0;
            }
            if(breakout){
                breakout=0;
                break;
            }
        }
        */

        //VERY SLOW, took 25 seconds with O3 native
        for(int m=0; m<=shuffle; m++){
            //permute/inc and bit move shuffle into mask1
            for(int mm=1; mm<=used; mm++) mask1[mm-1] = (m>>(used-mm)&0x00000001);      //this could be better
            for(int n=0; n<=shuffle; n++){
                //permute/inc and bit move shuffle into mask2
                for(int nn=1; nn<=used; nn++) mask2[nn-1] = (n>>(used-nn)&0x00000001);  //this could be better
                for(int l=0; l<used; l++){
                    //--> THERE WILL BE OVERLAPS
                    switch(mask1[l] + mask2[l]){
                        case(0):
                            hold *= nums[l];
                            break;
                        case(1):
                            hold += nums[l];                                       //allow overlapping of least expensive operation type
                            break;
                        case(2):
                            inter = nums[l];
                            while(inter>0){
                                digits++;
                                inter=inter/10;
                            }
                            for(int n=0; n<digits; n++) hold*=10; 
                            hold += nums[l];
                            digits=0;
                            inter=0;
                            break;
                        default:
                            printf("-- IMPOSSIBILITY!\n");
                            break;
                    }
                }//ends check loop
                if(hold==test){
                    sum+=test;
                    breakout=1;
                    break;
                }
                hold=0;
            }
            if(breakout){
                breakout=0;
                break;
            }
        }//for next line
        hold = 0;
        used = 0;
    }

    printf("> %ld", sum);
    endl;

    fclose(stream);
    return 0;
}
