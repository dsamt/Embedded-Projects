/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* HAL and Application includes */
#include <Application.h>
#include <HAL/HAL.h>
#include <HAL/Timer.h>

/**
 * The main entry point of your project. The main function should immediately
 * stop the Watchdog timer, call the Application constructor, and then
 * repeatedly call the main super-loop function. The Application constructor
 * should be responsible for initializing all hardware components as well as all
 * other finite state machines you choose to use in this project.
 *
 * THIS FUNCTION IS ALREADY COMPLETE. Unless you want to temporarily experiment
 * with some behavior of a code snippet you may have, we DO NOT RECOMMEND
 * modifying this function in any way.
 */
int main(void)
{
    // Stop Watchdog Timer - THIS SHOULD ALWAYS BE THE FIRST LINE OF YOUR MAIN
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.
    InitSystemTiming();

    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct();

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        Application_loop(&app, &hal);
        HAL_refresh(&hal);
    }
}

/**
 * A helper function which increments a value with a maximum. If incrementing
 * the number causes the value to hit its maximum, the number wraps around
 * to 0.
 */
uint32_t CircularIncrement(uint32_t value, uint32_t maximum)
{
    return (value + 1) % maximum;
}

/**
 * The main constructor for your application. This function should initialize
 * each of the FSMs which implement the application logic of your project.
 *
 * @return a completely initialized Application object
 */
Application Application_construct()
{
    Application app;

    app.firstCall = true;

    return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_loop(Application* app, HAL* hal)
{
    // The obligatory non-blocking check. At any time in your code, pressing and
    // holding Boosterpack S2 should always turn on Launchpad LED1.
    //
    // Not a real TODO: Ensure that your code never blocks turning on this LED.
    LED_turnOff(&hal->launchpadLED1);
    if (Button_isPressed(&hal->boosterpackS2)) {
        LED_turnOn(&hal->launchpadLED1);
    }

    // Update communications if either this is the first time the application is
    // run or if Boosterpack S1 is pressed.
    if (Button_isTapped(&hal->boosterpackS1) || app->firstCall) {
        Application_updateCommunications(app, hal);
    }

    // Interpret a new character if one is received.
    uint32_t interruptFlags = UART_getInterruptStatus(
            EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    bool charWasReceived = interruptFlags == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG;

    if (charWasReceived) {
        Application_interpretIncomingChar();
    }
}

/**
 * Updates which LEDs are lit and what baud rate the UART module communicates
 * with, based on what the application's baud choice is at the time this
 * function is called.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_updateCommunications(Application* app, HAL* hal)
{
    // When this application first loops, the proper LEDs aren't lit. The
    // firstCall flag is used to ensure that the
    if (app->firstCall) {
        app->firstCall = false;
    }

    // When Boosterpack S1 is tapped, circularly increment which baud rate is used.
    if (Button_isTapped(&hal->boosterpackS1))
    {
        uint32_t newBaudNumber = CircularIncrement((uint32_t) app->baudChoice, NUM_BAUD_CHOICES);
        app->baudChoice = (UART_Baudrate) newBaudNumber;
    }

    // Based on the new application choice, turn on the correct LED.
    // To make your life easier, we recommend turning off all LEDs before
    // selectively turning back on only the LEDs that need to be relit.
    // -------------------------------------------------------------------------
    LED_turnOff(&hal->boosterpackRed);
    LED_turnOff(&hal->boosterpackGreen);
    LED_turnOff(&hal->boosterpackBlue);

    // Turn on all appropriate LEDs according to the tasks below.
    // Not a real TODO: The TODOs are actually below here. However, this is to bring your attention to see how
    // we are using LEDs to debug the project. You can use this method in your future projects.
    // Turn on and off various LEDs to see if you are reaching a certain state or a condition is being met, etc.
    switch (app->baudChoice)
    {
        // When the baud rate is 9600, turn on Boosterpack LED Red
        case BAUD_9600:
            LED_turnOn(&hal->boosterpackRed);
            break;

        // TODO: When the baud rate is 19200, turn on Boosterpack LED Green
        case BAUD_19200:
            LED_turnOn(&hal->boosterpackGreen);
            break;

        // TODO: When the baud rate is 38400, turn on Boosterpack LED Blue
        case BAUD_38400:
            LED_turnOn(&hal->boosterpackBlue);
            break;

        // TODO: When the baud rate is 57600, turn on all Boosterpack LEDs (white color illuminates)
        case BAUD_57600:
            LED_turnOn(&hal->boosterpackRed);
            LED_turnOn(&hal->boosterpackGreen);
            LED_turnOn(&hal->boosterpackBlue);
            break;

        // In the default case, this program will do nothing.
        default:
            break;
    }

    // Baud Rate generation
    //
    // You recall that we have to determine UCBR, UCBRF and UCBRS
    // On the software side, these registers are referred as clockPrescalar,
    // firstModReg and secondModReg in the configuration struct.
    // Our goal is to calculate these register values for all needed Baudrates.
    // The first one is given to you.
    // You can calculate these and put them in the provided arrays.
    // The first element of all three arrays belong to 9600.
    // The second elements of all arrays belong to 19200 and so on.
    // When using the baudChoice to index the array, we are effectively saying
    // that different baud rates choices correspond with different numbers
    // without requiring excessive amounts of if-statements.

    // TODO: Determine the correct values for the next three elements of each array.
    // -------------------------------------------------------------------------
    uint32_t clockPrescalerMapping[NUM_BAUD_CHOICES] = { 312, 156, 78, 52};
    uint32_t clockPrescaler = clockPrescalerMapping[app->baudChoice];

    uint32_t firstModRegMapping[NUM_BAUD_CHOICES] = { 8, 4, 2, 1};
    uint32_t firstModReg = firstModRegMapping[app->baudChoice];

    uint32_t secondModRegMapping[NUM_BAUD_CHOICES] = { 0, 0, 0, 0};
    uint32_t secondModReg = secondModRegMapping[app->baudChoice];


    // Initialize the main UART Config from TI's Driverlib
    // -------------------------------------------------------------------------
    eUSCI_UART_ConfigV1 config;

    // We are using the sub-main clock (SMCLK Clock Source)
    config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;


    config.clockPrescalar = clockPrescaler;
    config.firstModReg    = firstModReg;
    config.secondModReg   = secondModReg;

    // Configure the rest of the UART struct members
    config.parity           = EUSCI_A_UART_NO_PARITY;    // No Parity
    config.msborLsbFirst    = EUSCI_A_UART_LSB_FIRST;    // LSB First
    config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT; // One Stop Bit
    config.uartMode         = EUSCI_A_UART_MODE;         // UART mode
    config.dataLength       = EUSCI_A_UART_8_BIT_LEN;    // Data length
    config.overSampling     = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    // END the Config Initialization
    // -------------------------------------------------------------------------

    // Invoke system driverlib calls which actually initialize the hardware
    // module to accept UART input from your USB.
    UART_initModule(EUSCI_A0_BASE, &config);
    UART_enableModule(EUSCI_A0_BASE);
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
}

/**
 * Interprets a character which was incoming and returns an interpretation of
 * that character.
 */
void Application_interpretIncomingChar()
{
    // The character received from your serial terminal
    char rxChar = UART_receiveData(EUSCI_A0_BASE);

    // The character to return back to sender. By default, we assume the letter
    // to send back is an 'O' (assume the character is an "other" character)
    char txChar = 'O';

    // Numbers - if the character entered was a number, transfer back an 'N'
    if (rxChar >= '0' && rxChar <= '9') {
        txChar = 'N';
    }

    // Letters - if the character entered was a letter, transfer back an 'L'
    if ((rxChar >= 'a' && rxChar <= 'z') || (rxChar >= 'A' && rxChar <= 'Z')) {
        txChar = 'L';
    }

    // Only send a character if the UART module can send it back
    uint32_t interruptFlags = UART_getInterruptStatus(
            EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
    bool canSendChar = interruptFlags == EUSCI_A_UART_TRANSMIT_INTERRUPT;

    if (canSendChar) {
        UART_transmitData(EUSCI_A0_BASE, txChar);
    }
}
