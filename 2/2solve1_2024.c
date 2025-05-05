#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ifint(a)        if((a>=48)&&(a<=57))

int main(int argc, char* argv[])
{
    int handler;
    int readRes=1;
    long curNum = 0;   //if var > 2147483648 => goes negative if signed 32bits int. Also (int) overrides initial long declaration
                        //ALSO, maybe look for another way to be able to use "unsigned int" since using "long" bloats up memory usage
    unsigned char readChar;
    short report[30];       //more memory efficient but slower than full matrix ++ signed
    int sum = 0;
    unsigned char used = 0;

    handler = open(argv[1], O_RDONLY);      //unsafe input

    //parse reports
    while(readRes!=0)
    {
        while((readChar!='\n')&&(readRes)){                    //somehow '' is ok for this type of op, but "" ISN'T
            readRes = read(handler, &readChar, 1);
            ifint(readChar){
                curNum = (10*curNum*(curNum>-1)) + readChar-48;
            }
            else if(curNum!=0){
                report[used++] = curNum;
                curNum = 0;
            }
        }
        if(!readRes) break;
        //handle current report
        sum++;      //preemptive. Deduce this if !condition
        if((report[0]>report[1])&&(report[0]-report[1]<4)){
            for(int i=1; i<used-1; i++){
                if((report[i]<=report[i+1])||(report[i]-report[i+1]>3)){
                    sum--;
                    break;
                }
            }
        }
        else if((report[0]<report[1])&&(report[1]-report[0]<4)){
            for(int i=1; i<used-1; i++){
                if((report[i+1]<=report[i])||(report[i+1]-report[i]>3)){
                    sum--;
                    break;
                }
            }
        }
        else{
            sum--;
        }
        used = 0;
        readChar=0;
    }

    printf("%d\n", sum);

    return 0;
}
