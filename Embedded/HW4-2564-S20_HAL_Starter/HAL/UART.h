/*
 * UART.h
 *
 *  Created on: Dec 31, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// Ever since the new API update in mid 2019, the UART configuration struct's
// name changed from [eUSCI_UART_Config] to [eUSCI_UART_ConfigV1]. If your code
// does not compile, uncomment the alternate #define and comment the original
// one.
// -----------------------------------------------------------------------------
// #define UART_Config eUSCI_UART_Config
#define UART_Config eUSCI_UART_ConfigV1

// An enum outlining what baud rates the UART_construct() function can use in
// its initialization.
enum _UART_Baudrate
{
    BAUD_9600, BAUD_19200, BAUD_38400, BAUD_57600, NUM_BAUD_CHOICES
};
typedef enum _UART_Baudrate UART_Baudrate;

// TODO: Write an overview explanation of what this UART struct does, and how it
//       interacts with the functions below. Consult <HAL/Button.h>,
//       <HAL/LED.h>, and <HAL/Timer.h> for examples on how to do this.
//The struct is created for the UART object and should not be used to be called in functions and
//should be treated as private members that should not be accessed outside of the struct
//This struct allows for things to be called with UART_(function), which is how the functions interact with
//struct. Like with all the HAL functions below that are called with UART_getChar, UART_hasChar, etc.
//these fucntions all use member types from this struct and all the fucntions can access these variables
//when higher level functions are used like UART_receiveData(uart->moduleInstance).
struct _UART
{
    UART_Config config;

    uint32_t moduleInstance;
    uint32_t port;
    uint32_t pins;
};
typedef struct _UART UART;

// Initializes a UART connection.
UART UART_construct(UART_Baudrate baudChoice);

// TODO: Write a comment which explains what each of these functions does. In the
//       header, prefer explaining WHAT the function does, as opposed to HOW it is
//       implemented.
//UART_getChar receives whatever character is pressed by the user and returns that character
//UART_hasChar makes sure that a character is actually being sent and not something else
//UART_canSend makes sure that there is nothing interrupting the transmission of data and returns true
//if there is no interrupt and that data can be sent
//UART_putChar sends the character back
char UART_getChar(UART* uart);
bool UART_hasChar(UART* uart);
bool UART_canSend(UART* uart);
void UART_putChar(UART* uart, char c);

// Updates the UART baudrate to use the new baud choice.
void UART_updateBaud(UART* uart, UART_Baudrate baudChoice);

#endif /* HAL_UART_H_ */
