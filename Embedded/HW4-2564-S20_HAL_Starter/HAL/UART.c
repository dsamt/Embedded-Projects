/*
 * UART.c
 *
 *  Created on: Dec 31, 2019
 *      Author: Matthew Zhong
 *  Supervisor: Leyla Nazhand-Ali
 */

#include <HAL/Timer.h>
#include <HAL/UART.h>

/**
 * Initializes the UART module using a specified baudrate as well as a
 * secondModReg as specified through the table provided in the MSP432 User
 * Guide.
 *
 * @param baudRate:         The baudrate to which the UART module is configured
 * @param secondModReg:     A special value from a table in the MSP432 datasheet
 *
 * @return the completed UART object
 */
UART UART_construct(UART_Baudrate baudChoice)
{
    // Create the UART struct
    UART uart;

    // Initialize auxiliary members of the struct
    // -------------------------------------------------------------------------
    uart.moduleInstance = EUSCI_A0_BASE;    // Module is always EUSCI_A0_BASE
    uart.port = GPIO_PORT_P1;                          // TODO: Copy over the port for USB0
    uart.pins = (GPIO_PIN2 | GPIO_PIN3);                 // TODO: Copy over the pins for USB0

    // Baud Rate Mapping - Map each baud choice enum to an actual integer.
    //
    // When using the baudChoice to index the array, we are effectively saying
    // that different baud rates choices correspond with different numbers
    // without requiring excessive amounts of if-statements.
    //
    // TODO: Copy from your noHAL solution that you are confident is working
    uint32_t clockPrescalerMapping[NUM_BAUD_CHOICES] = { 312, 156, 78, 52};
        uint32_t clockPrescaler = clockPrescalerMapping[baudChoice];

        uint32_t firstModRegMapping[NUM_BAUD_CHOICES] = { 8, 4, 2, 1};
        uint32_t firstModReg = firstModRegMapping[baudChoice];

        uint32_t secondModRegMapping[NUM_BAUD_CHOICES] = { 0, 0, 0, 0};
        uint32_t secondModReg = secondModRegMapping[baudChoice];
    // Initialize the main UART Config from TI's Driverlib
    // -------------------------------------------------------------------------

    // We are using the sub-main clock (SMCLK Clock Source)
    uart.config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;

    // TODO: Replace 0s with the correct variables from above
    uart.config.secondModReg = secondModReg;
    uart.config.clockPrescalar = clockPrescaler;
    uart.config.firstModReg    = firstModReg;

    // TODO: Determine what other variables need to be initialized in the struct
    //       and initialize them.
    uart.config.parity           = EUSCI_A_UART_NO_PARITY;    // No Parity
    uart.config.msborLsbFirst    = EUSCI_A_UART_LSB_FIRST;    // LSB First
    uart.config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT; // One Stop Bit
    uart.config.uartMode         = EUSCI_A_UART_MODE;         // UART mode
    uart.config.dataLength       = EUSCI_A_UART_8_BIT_LEN;    // Data length
    uart.config.overSampling     = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    // END the Config Initialization
    // -------------------------------------------------------------------------

    // Invoke system driverlib calls which actually initialize the hardware
    // module to accept UART input from your USB.
    UART_initModule(uart.moduleInstance, &uart.config);
    UART_enableModule(uart.moduleInstance);
    GPIO_setAsPeripheralModuleFunctionInputPin(
            uart.port, uart.pins, GPIO_PRIMARY_MODULE_FUNCTION);

    // Return the completed UART instance
    return uart;
}

/**
 * Determines if the user has sent a UART data packet to the board by checking
 * the interrupt status of the proper system UART module.
 *
 * @param uart: The UART instance with which to handle our operations.
 *
 * @return true if the user has entered a character, and false otherwise
 */
bool UART_hasChar(UART* uart)
{
    uint8_t interruptStatus = UART_getInterruptStatus(
            uart->moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

    return interruptStatus == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG;
}

// TODO: Complete the UART_getChar() function.
// TODO: Write a descriptive comment explaining HOW the function is implemented.
//       In the implementation file, prefer explaining HOW the function is
//       implemented over simply WHAT the function does.
//This function uses a high level function to return what data was received and uses whatever module the
//user is using as a base for what was received
char UART_getChar(UART* uart)
{
    char rxChar = UART_receiveData(uart->moduleInstance);

    return rxChar;
}

// TODO: Complete the UART_canSend() function.
// TODO: Write a descriptive comment explaining HOW the function is implemented.
//       In the implementation file, prefer explaining HOW the function is
//       implemented over simply WHAT the function does.
//This function takes the Interrupt status and as long as there is no interrupt then the variable
//canSendChar is marked as true and the function returns true as long as there is no interrupt
bool UART_canSend(UART* uart)
{
    uint32_t interruptFlags = UART_getInterruptStatus(
                uart->moduleInstance, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    bool canSendChar = (interruptFlags == EUSCI_A_UART_TRANSMIT_INTERRUPT);
    return canSendChar;
}

// TODO: Complete the UART_putChar() function.
// TODO: Write a descriptive comment explaining HOW the function is implemented.
//       In the implementation file, prefer explaining HOW the function is
//       implemented over simply WHAT the function does.
//This function uses a high level function to take a character that is called from the module that the user
//wants and sends it back
void UART_putChar(UART* uart, char c)
{
     UART_transmitData(uart->moduleInstance, c);
}

/**
 * Reinitializes the UART module to use a new baud rate. For simplicity reasons,
 * it is easier to simply reconstruct the module as a new UART object.
 *
 * @param uart:         The module to reinitialize
 * @param baudChoice:   The new baud choice with which to update the module
 */
void UART_updateBaud(UART* uart, UART_Baudrate baudChoice)
{
    *uart = UART_construct(baudChoice);
}
