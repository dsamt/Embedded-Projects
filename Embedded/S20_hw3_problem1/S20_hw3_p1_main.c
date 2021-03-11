// This is so that we can use printf
#include <stdio.h>

// This is so that we can use BIT0, BIT1, ... macros as well as types bool, keywords true, false
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


void main() {

    // You can change starting_value to different values and test the code you write below to check if it works for all cases.
    unsigned int starting_value = 0XABCDEF01;
    bool test_v = false;

    unsigned k = starting_value;
    /* Example: In the below example we clear all the bits in k except for the LSB, or Bit 0 and store the result in a new variable l.
     * For this purpose, we use BIT0 mask.
     * To see where this macro is defined and what it means, hold the Ctrl key on your keyboard and click on BIT0 word below.
     * Optional: Debug the code without changing anything and see the values for k and l.
     */
    //unsigned int l = k & (~BIT0);


    /*
     * TODO #1: Write a snippet of code that will cause the bits 5 and 3 of k to be set to 1,
     * without affecting any other bits of k. 5 points
     */
    k = k | (BIT3 | BIT5);

    /*
     * TODO #2: Write a snippet of code that will cause the most-significant byte of k to contain the bit pattern 10010010,
     * without affecting any other bits of k. 5 points
     */
    k = k & 0x00FFFFFF;
    k = k | 0x92000000;


    /*
     * TODO #3: Write a snippet of code that toggles bit 2, sets bit 0 to 1, and resets bit 1 to 0. 5 points
     */
    k  = k ^ BIT2;
    k |= BIT0;
    k &= ~BIT1;

    /*
     * TODO #4: Write the expression that computes variable test_v, the Boolean value that is True if MSB of k is 1 and LSB of k is 0.
     * The expression should compute to False, otherwise. You need to remove the word "true" that is assigned to test_v and replace it
     * with some other expression. This expression should be true when MSB of k is 1 and its LBS is 0. 5 points
     */
   
   // test_v is true if k's MSB is 1 and LSB is 0;
    unsigned MSB = k & 0x80000000;
    unsigned LSB = k & 0x00000001;
            test_v = MSB & ~LSB;

    if (test_v)
        printf("The test variable is true.\n");
    else
        printf("The test variable is false.\n");



}
