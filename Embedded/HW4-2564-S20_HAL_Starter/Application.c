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
 * each of the FSMs which implement the application logic of your project
 *
 * @return a completely initialized Application object
 */
Application Application_construct()
{
    Application app;

    // Initialize local application state variables here!
    app.baudChoice = BAUD_9600;
    app.firstCall = true;

    return app;
}

/**
 * The main super-loop function of the application. We place this inside of a
 * single infinite loop in main. In this way, we can model a polling system of
 * FSMs. Every cycle of this loop function, we poll each of the FSMs one time,
 * followed by refreshing all inputs to the system through a convenient
 * [HAL_refresh()] call
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object.
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
    if (UART_hasChar(&hal->uart)) {
        Application_interpretIncomingChar(app, hal);
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

    // Update the baud rate according to the one set above.
    UART_updateBaud(&hal->uart, app->baudChoice);

    // Based on the new application choice, turn on the correct LED.
    // To make your life easier, we recommend turning off all LEDs before
    // selectively turning back on only the LEDs that need to be relit.
    // -------------------------------------------------------------------------
    LED_turnOff(&hal->boosterpackRed);
    LED_turnOff(&hal->boosterpackGreen);
    LED_turnOff(&hal->boosterpackBlue);

    // TODO: Turn on all appropriate LEDs according to the tasks below.
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

        // TODO: When the baud rate is 57600, turn on all Boosterpack LEDs (illuminates white)
        case BAUD_57600:
            LED_turnOn(&hal->boosterpackRed);
            LED_turnOn(&hal->boosterpackGreen);
            LED_turnOn(&hal->boosterpackBlue);
            break;

        // In the default case, this program will do nothing.
        default:
            break;
    }
}

/**
 * Interprets a character which was incoming and sends an interpreted character
 * back if the system USB UART module can accept a new character.
 *
 * @param app:  A pointer to the main Application object.
 * @param hal:  A pointer to the main HAL object
 */
void Application_interpretIncomingChar(Application* app, HAL* hal)
{
    // The character received from your serial terminal
    char rxChar = UART_getChar(&hal->uart);

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
    if (UART_canSend(&hal->uart)) {
        UART_putChar(&hal->uart, txChar);
    }
}
