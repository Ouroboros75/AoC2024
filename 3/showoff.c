#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    char *mask = "mul(,)", *maskDo = "do()", *maskDont = "don't()";
    char dewit=1;
    unsigned char readChar;
    int readRes = 1, handler = 0, toggle = 0, curNum = 0;
    unsigned int nums[2] = {0};
    long magic = 0, magic0 = 0, magicDo = 0, magicDont = 0;
    unsigned long sum = 0;
    //cookin up the magic formula
    for(int i=0; i<7; i++){
        magicDont += maskDont[i];
        magicDont<<=8;
        if(i<4){
            magicDo += maskDo[i];
            magicDo<<=8;
        }
        if(i<6){
            magic0 += mask[i];
            magic0<<=8;
        }
    }
    handler = open(argv[1], O_RDONLY);      //unsafe input
    while(readRes!=0){
        readRes = read(handler, &readChar, 1);
        if((readChar>=48)&&(readChar<=57)) curNum = (10*curNum) + readChar-48;
        else{
            magic += readChar; 
            magic<<=8;
            nums[toggle]=curNum;
            toggle=!toggle;
            curNum=0;
            if(readChar=='m'){
                memset(nums, 0, 2*(sizeof(int)/8));
                continue;
            }
            else if(readChar==')'){
                // part2 exclusive - START
                if(((magic&0x000000FFFFFFFF00) ^ magicDo) == 0) dewit=1;
                else if(((magic&0xFFFFFFFFFFFFFF00) ^ magicDont) == 0) dewit=0;
                // part2 exclusive - END
                if((nums[0]>0)&&(nums[1]>0)&&dewit){
                    if(((magic&0x00FFFFFFFFFFFF00) ^ magic0) == 0){
                        printf("%d * %d\n", nums[0], nums[1]);
                        sum += (nums[0] * nums[1]);
                    }
                }
            }
        }
        if(!readRes) break;
    }
    printf("== %ld\n", sum);
    return 0;
}
