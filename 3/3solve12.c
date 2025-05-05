#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ifint(a)        if((a>=48)&&(a<=57))
#define arrType         int                  //must be a predefined type
#define initLen         10
#define endl            printf("\n")

/* DEBUGGING
for(int i=7; i>=0; i--)
{
    f = ((char)(magic>>(8*i)) > 0) ? magic>>(8*i) : 167;
    printf("%c", f);
}
endl;
magic&=0x00FFFFFFFFFFFF00;
for(int i=7; i>=0; i--)
{
    f = ((char)(magic>>(8*i)) > 0) ? magic>>(8*i) : 167;
    printf("%c", f);
}
endl;
for(int i=7; i>=0; i--)
{
    f = ((char)(magic0>>(8*i)) > 0) ? magic0>>(8*i) : 167;
    printf("%c", f);
}
endl;
*/

int main(int argc, char* argv[])
{
    int handler;
    int readRes=1;
    int curNum = 0;
    unsigned char readChar;
    char *mask = "mul(,)";
    char *maskDo = "do()";
    char *maskDont = "don't()";
    int toggle = 0;
    unsigned int nums[2] = {0};
    unsigned long sum = 0;

    long magic = 0;
    long magic0 = 0; 
    long magicDo = 0; 
    long magicDont = 0; 
    char f;
    char dewit=1;
    //cookin up the magic formula
    for(int i=0; i<6; i++)
    {
        magic0 += mask[i];
        magic0<<=8;
        printf("%c ", mask[i]);
    }
    for(int i=0; i<4; i++)
    {
        magicDo += maskDo[i];
        magicDo<<=8;
        printf("%c ", maskDo[i]);
    }
    for(int i=0; i<7; i++)
    {
        magicDont += maskDont[i];
        magicDont<<=8;
        printf("%c ", maskDont[i]);
    }
    magic = magic0;
    //magic &= ~(((long)(255) << 56) | 255);  //ALTERNATIVE WAY TO APPLY MASK. printf() does not show first eight '0' bits
    magic |= (((long)(255) << 56) | 255);
    magic &= 0x00FFFFFFFFFFFF00;
    endl;
    for(int i=7; i>=0; i--)
    {
        f = ((char)(magic0>>(8*i)) > 0) ? magic0>>(8*i) : 167;
        printf("%c", f);
    }
    endl;
    if(((magic&0x00FFFFFFFFFFFF00) ^ magic0) == 0) printf(" magnifico!");
    for(int i=7; i>=0; i--)
    {
        f = ((char)(magic0>>(8*i)) > 0) ? magic0>>(8*i) : 167;
        printf("%c", f);
    }
    endl;
    magic = 0;
    
    //Start-up
    handler = open(argv[1], O_RDONLY);      //unsafe input
    while(readRes!=0)
    {
        readRes = read(handler, &readChar, 1);
        ifint(readChar){
            curNum = (10*curNum) + readChar-48;
        }
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
                if(((magic&0x000000FFFFFFFF00) ^ magicDo) == 0){
                    dewit=1;
                }
                else if(((magic&0xFFFFFFFFFFFFFF00) ^ magicDont) == 0){
                    dewit=0;
                }
                // part2 exclusive - END
                else if((nums[0]>0)&&(nums[1]>0)&&dewit){
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
