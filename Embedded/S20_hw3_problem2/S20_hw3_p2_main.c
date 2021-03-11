/*
 * This project is related to the problem 2 of HW3, ECE2564 Spring 2020.
 * It is important that you read the PDF homework description to understand what you need to do.
 * The comments and TODO items in this document are not self-sufficient to tell you how to complete this project.
 */


#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


// TODO #1: complete the below bitmasks. Intensity mask is given to you as an example.
// You need to change the rest from 0 to the appropriate value.
#define INTENSITY_MASK (BIT6 | BIT7)
#define COLOR_MASK (BIT4 | BIT5)
#define FLICKER_MASK (BIT3)

//-----------------------------------
// TODO #2: complete the below bitmask positions. Intensity mask position is given to you as an example.
// You need to change the rest from 0 to the appropriate value.
// The position of the mask is the index of the lowest bit of the mask.
#define INTENSITY_BIT_POS 6
#define COLOR_BIT_POS 4
#define FLICKER_BIT_POS 3

//-----------------------------------
// TODO #3: Use typedef and enum keywords to define three new types called color_t, intensity_t, and flickering_t.
// These new types enumerate different color, intensity, and flickering possibilities.
// color_t is done for you as an example.
typedef enum {BLUE, GREEN, RED, YELLOW} color_t;

typedef enum {INT_OFF, INT_LOW, INT_MED, INT_HIGH} intensity_t;

typedef enum {FLICK_OFF, FLICK_ON} flickering_t;

//-----------------------------------
// TODO #NAN: There is no action item here for you except for reading the below line.
// In here we are defining a new type LSR_t just to add readability to our code.
typedef unsigned char LSR_t;


//-----------------------------------
// TODO #4: Create a structure that hold the data needed for each light.
// Most of the work is done for you. All you need is to study this and add one element that is missing.
typedef struct {
    LSR_t lightSetting;           // the setting of the light
    unsigned int x;               // the x coordinate of the light
    unsigned int y;               // the y coordinate of the light
} light_t;


// function declarations

// These three functions get an LSR as input and return its separate settings
color_t getColorSetting(LSR_t inLSR);
intensity_t getIntensitySetting(LSR_t inLSR);
flickering_t getFlickeringSetting(LSR_t inLSR);

// These three functions update a specific setting within a given LSR (input is the pointer to the LSR)
void updateIntensity(LSR_t* LSR_p, intensity_t newIntensity);
void updateColor(LSR_t* LSR_p, color_t newColor);
void updateFlicker(LSR_t* LSR_p, flickering_t newFlicker);

// This function gets three different settings of an LSR and creates an LSR out of it
LSR_t makeLSR(intensity_t newIntensity, color_t newColor, flickering_t newFlickering);

// This function dims the light intensity for the LSR pointed by the LSR_p
void dimLight(LSR_t *LSR_p);



void main() {

    LSR_t tempLSR = 0xBE;

    /* TODO #5:After you implement getColorSetting, getIntensitySetting and getFlickerSetting (find them below main)
     * debug the code until it says "End of TODO #5" with various values instead of 0x68 for tempLSR and make sure
     * your getColorSetting, getIntensitySetting, and getFlickeringSetting functions are working correctly.
     */
    color_t color = getColorSetting(tempLSR);
    intensity_t intensity = getIntensitySetting(tempLSR);
    flickering_t flickering = getFlickeringSetting(tempLSR);
    // End of TODO #5

    /* TODO #6: After you implement updateColorSetting, updateIntensitySetting, updateFlickerigSetting (find them below main)
     *  debug the below three lines.
     */
    updateIntensity(&tempLSR, INT_HIGH);
    updateColor(&tempLSR, BLUE);
    updateFlicker(&tempLSR, FLICK_OFF);
    // End of TODO #6

    /* TODO #7: After you implement makeLSR (find it below main), debug the below line. (Use breakpoints to skip the top part.
     * Change the input setting to other things and make sure makeLSR works properly. (this will be different rounds of debugging.)
     */
    LSR_t myLSR = makeLSR(INT_HIGH, BLUE, FLICK_OFF);
    // End of TODO #7

    /* TODO #8: Develop the code for dimLight (find it below main)
     * Debug the code.
     */
    dimLight(&myLSR);
    intensity_t newIntensity = getIntensitySetting(myLSR);
    // End of TODO #8

    // The below lines initializes the light system of a stage with 5 lights.

   light_t stageLights[5];
   stageLights[0].lightSetting = makeLSR(INT_HIGH, BLUE, FLICK_ON);
   stageLights[1].lightSetting = makeLSR(INT_LOW, BLUE, FLICK_ON);
   stageLights[2].lightSetting = makeLSR(INT_MED, BLUE, FLICK_ON);
   stageLights[3].lightSetting = makeLSR(INT_OFF, BLUE, FLICK_ON);
   stageLights[4].lightSetting = makeLSR(INT_OFF, BLUE, FLICK_ON);

   stageLights[0].x = 7;
   stageLights[1].x = 10;
   stageLights[2].x = 5;
   stageLights[3].x = 40;
   stageLights[4].x = 20;
//* comment out after you have added y to the struct.
   stageLights[0].y = 10;
   stageLights[1].y = 8;
   stageLights[2].y = 20;
   stageLights[3].y = 30;
   stageLights[4].y = 40;


   /* TODO #9: Write a snippet of code that if the x and y coordinates of a light are both less than 20, that light is dimmed.
    *  Also, if the x coordinate or the y coordinate are more than 30, the color should be blue, else yellow.
    * You have to use the functions you developed earlier to achieve this.
    */
   unsigned int n;

   for (n = 0; n<5; n++){
   if (stageLights[n].x < 20 && stageLights[n].y < 20){
           dimLight(&myLSR);
       }
       else if (stageLights[n].x > 30 || stageLights[n].y > 30){
           updateColor(&myLSR, BLUE);
       }
       else {
           updateColor(&myLSR, YELLOW);
       }

   }
}



/*
 * input:
 *          LSR_t
 * output:
 *          color_t
 * effect:
 *          This function isolates the color setting of LSR and returns it
 */
color_t getColorSetting(LSR_t inLSR)
{
    // The below line creates a warning because the right side of the assignment is not of type color_t.
    // If you like you can typecast the right side to color_t
    // The typecasted version is like this:
    // color_t colorSetting = (color_t) ((inLSR & COLOR_MASK) >> COLOR_BIT_POS);
    color_t colorSetting = (color_t) ((inLSR & COLOR_MASK) >> COLOR_BIT_POS);

    return colorSetting;
}


/*
 * input:
 *          LSR_t
 * output:
 *          intensity_t
 * effect:
 *          This function isolates the intensity setting of LSR and returns it
 */
intensity_t getIntensitySetting(LSR_t inLSR) {

    intensity_t IntensitySetting = (intensity_t) ((inLSR & INTENSITY_MASK) >> (INTENSITY_BIT_POS));

    return IntensitySetting;
}

/*
 * input:
 *          LSR_t
 * output:
 *          flickering_t
 * effect:
 *          This function isolates the flickering setting of LSR and returns it
 */
flickering_t getFlickeringSetting(LSR_t inLSR) {


    flickering_t FlickeringSetting = (flickering_t) ((inLSR & FLICKER_MASK) >> (FLICKER_BIT_POS));


    return FlickeringSetting;
}


/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and intensity_t (the new intensity)
 * output:
 *          nothing
 * effect:
 *          This function updates the intensity of the LSR pointed by LSR_p to the new intensity
 */

void updateIntensity(LSR_t* LSR_p, intensity_t newIntensity) {
    if (newIntensity == INT_OFF){
        *LSR_p &= ~(BIT7 | BIT6);
    }
    if (newIntensity == INT_LOW){
        *LSR_p &= ~BIT7;
        *LSR_p |= BIT6;
    }
    if (newIntensity == INT_MED){
        *LSR_p |= BIT7;
        *LSR_p &= ~BIT6;
    }
    if (newIntensity == INT_HIGH){
            *LSR_p |= (BIT7 | BIT6);
        }
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and color_t (the new color)
 * output:
 *          nothing
 * effect:
 *          This function updates the color of the LSR pointed by LSR_p to the new color
 */
void updateColor(LSR_t* LSR_p, color_t newColor) {
    if (newColor == BLUE) {
        *LSR_p &= ~(BIT5 | BIT4);
    }
    if (newColor == GREEN){
    *LSR_p &= ~BIT5;
    *LSR_p |= BIT4;
}
    if (newColor == RED){
    *LSR_p |= BIT5;
    *LSR_p &= ~BIT4;
}
    if (newColor == YELLOW){
        *LSR_p |= (BIT5 | BIT4);
    }
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that needs to be updated) and flickering_t (the new flickering)
 * output:
 *          nothing
 * effect:
 *          This function updates the flickering of the LSR pointed by LSR_p to the new flickering
 */
void updateFlicker(LSR_t* LSR_p, flickering_t newFlicker) {
        if (newFlicker == FLICK_OFF){
            *LSR_p &= ~(BIT3);
            *LSR_p &= ~(BIT2 | BIT1 | BIT0);
        }
        if (newFlicker == FLICK_ON){
            *LSR_p |= BIT3;
            *LSR_p &= ~(BIT2 | BIT1 | BIT0);
        }
}


/*
 * input:
 *          intensity_t, color_t and flickering_t of the to-be-constructed LSR
 * output:
 *          LSR_t
 * effect:
 *          This function receives three integers representing lamp flickering status, color, and intensity and returns the LSR value
 *          that will create that light combination. Use 0s for unused bits in the LSR.
 *          For example, makeLSR with medium intensity, blue color and no flickering returns 0x80
 */
LSR_t makeLSR(intensity_t newIntensity, color_t newColor, flickering_t newFlickering) {
        LSR_t makeLSR = 0x00;

        if (newColor == BLUE) {
           makeLSR &= ~(BIT5 | BIT4);
           }
           if (newColor == GREEN){
           makeLSR &= ~BIT5;
           makeLSR |= BIT4;
       }
           if (newColor == RED){
           makeLSR |= BIT5;
           makeLSR &= ~BIT4;
       }
           if (newColor == YELLOW){
               makeLSR |= (BIT5 | BIT4);
           }
           if (newIntensity == INT_OFF){
               makeLSR &= ~(BIT7 | BIT6);
           }
           if (newIntensity == INT_LOW){
               makeLSR &= ~BIT7;
               makeLSR |= BIT6;
           }
           if (newIntensity == INT_MED){
               makeLSR |= BIT7;
               makeLSR &= ~BIT6;
           }
           if (newIntensity == INT_HIGH){
                makeLSR |= (BIT7 | BIT6);
               }

        if (newFlickering == FLICK_OFF){
                    makeLSR &= ~(BIT3);
                    makeLSR &= ~(BIT2 | BIT1 | BIT0);
                }
        if (newFlickering == FLICK_ON){
                    makeLSR |= BIT3;
                    makeLSR &= ~(BIT2 | BIT1 | BIT0);
                }
        return makeLSR;
}

/*
 * input:
 *          LSR_t * (the pointer to the LSR that its light intensity should be reduced
 * output:
 *          none
 * effect:
 *          The intensity setting of the light that the pointer is pointing to is reduced by one level. If it is OFF, it stays OFF.
 */
void dimLight(LSR_t *LSR_p) {
        intensity_t Intensity = getIntensitySetting(*LSR_p);

        if (Intensity == INT_HIGH){
            updateIntensity(LSR_p, INT_MED);
        }
        if (Intensity == INT_MED){
            updateIntensity(LSR_p, INT_LOW);
        }
        if (Intensity == INT_LOW){
            updateIntensity(LSR_p, INT_OFF);
        }
        if (Intensity == INT_OFF){
            updateIntensity(LSR_p, INT_OFF);
        }

}



