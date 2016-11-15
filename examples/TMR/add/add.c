/*
    Simple add for TMR test
*/

#include <stdio.h>

#define INPUTSIZE 1024
#define EXPECTED_TOTAL 2051

int a[INPUTSIZE] = {2,3,4,[3 ... 1023] = 2};

int main(){

    int output[INPUTSIZE];
    int i;
	unsigned int r = 0;

    loop: for (i = 0; i < INPUTSIZE; i++){
		r += a[i];
    }
	
    printf ("Result: %d\n", r);
    if (r == EXPECTED_TOTAL) {
        printf("RESULT: PASS\n");
    } else {
        printf("RESULT: FAIL\n");
    }
    return r;
}
