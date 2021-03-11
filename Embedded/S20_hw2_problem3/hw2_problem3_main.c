#include <stdio.h>


void make_time_string(int min, int sec, unsigned char *string);


int main(void)
{
    // This statement declares string as an array of unsigned characters
    // This string allocates 6 characters in the memory. This means the string can have up to 5 ascii characters.
    // This is because all C strings should end in the NULL character, i.e. 0
    unsigned char string[6];

    // Let's manually create a string
    string[0] = 'a';
    string[1] = 'b';
    string[2] = 'c';
    string[3] = 0; //the NULL or terminating character. This indicates the end of the string
    string[4] = 'd';
    string[5] = 'e';

    // prints a new line (goes to the next line)
    printf("\n");

    // prints the string. Notice how only "abc" is printed and the rest of the array is ignored when running this code.
    // This is because everything after the NULL character is not part of the string.
    printf((char *)string);
    printf("\n");

    // Let's manually create a string that represents a number
    string[0] = '1';
    string[1] = '2';
    string[2] = '3';
    string[3] = '4';
    string[4] = NULL; //the NULL or terminating character. This indicates the end of the string
    string[5] = '5';


    // prints the string. Notice how only "1234" is printed and the rest of the array is ignored when running this code.
    printf((char *)string);
    printf("\n");

    /*
         If you do not include a NULL character at the end of your string bad things can happen in your program.
         This bug can manifest itself in different ways depending on where and when it happens.
         When a C code that has strings in it is misbehaving,
         it is important to go back and make sure all the strings have NULL characters in the space allocated for the string.
     */


    // char is 8 bits and can be assigned any number 0 to 255. Here numeric1 is assigned 0.
    unsigned char numbers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // ascii in here is a character and we are assigning '0' to it. This means ascci1 character 0 (not number 0) is assigned to it.
    unsigned char asciis[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    // In the debugger's variables window, compare the content of the above two arrays. You must see a pattern that connects
    // each number to its ascii equivalent.

    make_time_string(34, 5, string);
    printf((char *)string);
    printf("\n");

    make_time_string(0, 0, string);
    printf((char *)string);
    printf("\n");

    make_time_string(59, 59, string);
    printf((char *)string);
    printf("\n");

    while (1) {
    }
}

/*******************************************************************
* NAME :            make_time_string(int min, int sec, unsigned char *string)
*
* DESCRIPTION :     Creates a string from two numbers to represent time in minute and seconds
*
* INPUTS :
*       PARAMETERS:
*           int     min        minutes as a number
*           int     sec        seconds as a number
*
* OUTPUTS :
*       PARAMETERS:
*           unsigned char    * string to hold the characters that show time
*
*       RETURN :
*            Type:   none
*
* PORCESS:
*       This function turns the min and sec into a string that shows time.
*
* EXAMPLES:
*       If min = 0 and sec = 0, the string becomes "00:00"
*       If min = 10 and sec = 5, the string becomes "10:05"
*
* NOTES:
*       This function does not check to see if the min and sec are between 0 and 59.
*       It simply converts them to string without checking
*
*/

void make_time_string(int min, int sec, unsigned char *string) {


    //TODO: Write this function according to the above specification and the description in the homework assignment.
char str[2];
char str2[2];

sprintf(str, "%d", min);
sprintf(str2, "%d", sec);
if (min < 10){
    str[1] = str[0];
    str[0] = '0';
    }
    if (sec < 10){
        str2[1] = str2[0];
        str2[0] = '0';
    }

    string[0] = 0 + str[0];
    string[1] = str[1];
    string[2] = ':';
    string[3] = 0 + str2[0];
    string[4] = str2[1];
    string[5] = NULL;

}

