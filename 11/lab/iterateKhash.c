#include <stdio.h>
#include <unistd.h>
#include "khash.h"

KHASH_MAP_INIT_INT64(64, unsigned long*)
khash_t(64) *h;

unsigned long recurve(unsigned long number, int remainingSteps){
    puts("aykroyd -1");
    int ret, is_missing;
    khiter_t k = kh_get(64, h, number);

    puts("aykroyd 0");
    if(kh_exist(h, k)){
        puts("hold on");
    }
    else{
        puts("praise be");
    }
    puts("aykroyd 1");

    return 12;
}

int main(int argc, char* argv[]){
    unsigned long arr[75] = {0};
    int ret, is_missing;
    khiter_t k;
    h = kh_init(64);
    k = kh_put(64, h, 48576, &ret);
    //for example value 48576 returns 252312 stones after 25 steps
    arr[25] = 252312;
    kh_value(h, k) = arr;

    k = kh_get(64, h, 48576);
    if(kh_exist(h, k)){
        kh_value(h, k)[21] = 241221;
        printf("%lu - %lu - %lu - %lu - %lu", kh_value(h, k)[21],kh_value(h, k)[22],kh_value(h, k)[23],kh_value(h, k)[24],kh_value(h, k)[25]);
        printf("\n");
        printf("%lu\n", recurve(1323,75));
    }

    return 0;

}
