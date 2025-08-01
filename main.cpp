#define F_CPU 16000000UL

#include "funsape/funsapeLibGlobalDefines.hpp"
#include "funsape/peripheral/funsapeLibInt0.hpp"
#include "funsape/peripheral/funsapeLibTimer1.hpp"
#include "funsape/peripheral/funsapeLibUsart0.hpp"
#include "funsape/peripheral/funsapeLibPcint2.hpp"
#include "funsape/peripheral/funsapeLibAdc.hpp"
#include "funsape/peripheral/funsapeLibTwi.hpp"
#include "pcd8544.hpp"

#define LM75_ADDRESS                    0x48                                    // LM75 I2C Address
#define TEMP_REG                        0x00                                    // Temperature register

#define CLEAR_LEDS                      clrMaskOffset(PORTD, 0x07, 5);          // Clear all 3 LEDs with a mask
#define ALCOHOL_THRESHOLD               350                                     // Analogic alcohol threshold read by sensor (MQ-3)
#define TEMP_THRESHOLD                  30                                      // Analogic temperature threshold read by sensor (LM75)

typedef struct {
    bool_t adcReady;
    bool_t systemLocked;
    bool_t hasLockedIndicatorShown;
    bool_t error;
    bool_t isTemperatureHigh;
    bool_t buttonPressed;
    bool_t hasCheckedAlcoholLevel;
} systemFlags_t;

enum class StatesMachine {
    BOOT = 0,
    CLEAR = 1,
    BUSTED = 2,
    ERROR = 3,
};

enum class ErrorCode {
    NONE = 0,
    TWI_INIT = 3,
    TEMP_READ = 4,
    DISPLAY_INIT = 5,
};

systemFlags_t systemFlags;
ErrorCode errorCode = ErrorCode::NONE;
Pcd8544 display;
vuint16_t adcValue = 0;
vuint16_t tempValue[2];

void handlePrintTemperature(void);
void handleValidateAlcoholLevel(void);
void handleValidateTemperatureLevel(void);
void handleResetDisplay(void);
void handleUpdateStatusOnDisplay(char *bufferStatus, uint8_t x, bool_t useRenderScreen = true);
void setError(ErrorCode code);

int main()
{
    // Initialize flags
    systemFlags.adcReady = false;
    systemFlags.systemLocked = false;
    systemFlags.hasLockedIndicatorShown = false;
    systemFlags.error = false;
    systemFlags.isTemperatureHigh = false;
    systemFlags.buttonPressed = false;
    systemFlags.hasCheckedAlcoholLevel = false;

    // Initialize variables
    StatesMachine statesMachine = StatesMachine::BOOT;

    // Configure outputs
    setBit(DDRC, PC1);                          // Debug LED
    setBit(DDRC, PC3);                          // Buzzer
    setMaskOffset(DDRD, 0x07, 5);               // Sets PD5, PD6 & PD7 as output LEDs

    // Configure USART0
    usart0.enableTransmitter();
    if(!usart0.init()) {
        systemHalt();
    }
    usart0.stdio();
    printf("DrunkBuster Debug Console...\r\n");

    // Configure TIMER1 500ms
    timer1.init(Timer1::Mode::CTC_OCRA, Timer1::ClockSource::PRESCALER_256);
    timer1.setCompareAValue(31250);
    timer1.setCompareBValue(31250);
    timer1.clearCompareBInterruptRequest();

    // Configure ADC0
    adc.init(Adc::Mode::AUTO_TIMER1_COMPB, Adc::Reference::POWER_SUPPLY, Adc::Prescaler::PRESCALER_128);
    adc.setChannel(Adc::Channel::CHANNEL_0);
    adc.clearInterruptRequest();
    adc.activateInterrupt();
    adc.enable();

    // Configure PCINT2
    pcint2.init(Pcint2::Pin::PIN_PCINT20);
    pcint2.clearInterruptRequest();
    pcint2.activateInterrupt();

    // Configure TWI
    if(!twi.init(100000)) {
        printf("Erro ao iniciar TWI\r\n");
        setError(ErrorCode::TWI_INIT);
    }
    twi.setDevice(LM75_ADDRESS);
    printf("Iniciando leitura LM75...\r\n");

    // Configure LCD Nokia 5110
    display.setPort(&DDRB, PB2, PB5, PB3, PB1, PB0);
    if(!display.init()) {
        setError(ErrorCode::DISPLAY_INIT);
    };

    char title[17] = "DRUNKBUSTER V1.0";
    char alcoholTitle[8] = "Alcohol";
    char temperatureTitle[5] = "Temp";
    char defaultAlcoholValue[5] = "0000";
    char defaultTemperatureValue[5] = "00.0";
    char bufferStatus[5] = "BOOT";

    display.clearScreen();
    display.setFont((uint8_t *)TinyFont);

    display.print(title, 0, 0);
    display.print(alcoholTitle, 0, 8);
    display.print(temperatureTitle, 40, 8);

    display.setFont((uint8_t *)SmallFont);
    display.print(defaultAlcoholValue, 0, 16);
    display.print(defaultTemperatureValue, 40, 16);
    display.print(bufferStatus, 5, 35);

    display.drawRectangle(0, 32, 40, 43);
    display.drawRectangle(46, 32, 80, 43);
    display.renderScreen();

    // Configure Push Button
    clrBit(DDRD, PD4);
    setBit(PORTD, PD4);                 // Pull Up

    // Activate global interruptions
    sei();

    while(1) {
        if(isBitSet(TIFR1, OCF0A)) {                            // Clears automatic ADC trigger flag
            setBit(TIFR1, OCF0A);                               // Clears flag by writing 1 (write-1-to-clear)
        }

        switch(statesMachine) {
        case StatesMachine::BOOT:
            delayMs(250);                                                       // Blinks BLUE LED 2 times
            setBit(PORTD, PD7);                                                 // ...
            delayMs(250);                                                       // ...
            clrBit(PORTD, PD7);                                                 // ...
            delayMs(250);                                                       // ...
            setBit(PORTD, PD7);                                                 // ...
            delayMs(250);                                                       // ...
            clrBit(PORTD, PD7);                                                 // ...
            delayMs(250);                                                       // ...

            statesMachine = StatesMachine::CLEAR;                               // Jumps to CLEAR state
            break;
        case StatesMachine::CLEAR:
            if(systemFlags.error) {
                statesMachine = StatesMachine::ERROR;
            }

            if(systemFlags.adcReady) {
                CLEAR_LEDS;                                                     // Clears LEDs
                setBit(PORTD, PD6);                                             // Activates GREEN LED

                // Prints ADC Value on Serial
                printf("===============================\r\n");
                printf("ADC MQ-3 Value: %u\r\n", adcValue);

                char bufferStatus[6] = "CLEAR";
                char bufferTempStatus[5];

                handleUpdateStatusOnDisplay(bufferStatus, 5, false);            // Prints "CLEAR" on display and does not render screen
                handlePrintTemperature();                                       // Prints LM75 value (temperature) on Serial
                handleValidateAlcoholLevel();                                   // Validates alcohol level
                handleValidateTemperatureLevel();                               // Validates temperature level

                if(systemFlags.isTemperatureHigh) {
                    strcpy(bufferTempStatus, "HIGH");                           // Copies "HIGH" to bufferTempStatus
                    display.print(bufferTempStatus, 51, 35);                    // Shows "HIGH" on temperature rectangle in display
                } else {
                    strcpy(bufferTempStatus, "OK");                             // Copies "OK" to bufferTempStatus
                    display.print(bufferTempStatus, 56, 35);                    // Shows "OK" on temperature rectangle in display
                }

                display.renderScreen();
                systemFlags.adcReady = false;
            }

            if(systemFlags.systemLocked) {
                statesMachine = StatesMachine::BUSTED;                          // Jumps to BUSTED state
            }
            break;
        case StatesMachine::BUSTED:
            if(systemFlags.systemLocked) {                                      // User has not pressed unlock push button yet
                if(!systemFlags.hasLockedIndicatorShown) {                      // Runs only one time and waits for user to unlock system
                    printf("SYSTEM LOCKED\r\n");

                    timer1.setClockSource(Timer1::ClockSource::DISABLED);       // Disables TIMER1 Clock

                    // handleResetDisplay();
                    char bufferStatus[7] = "BUSTED";
                    handleUpdateStatusOnDisplay(bufferStatus, 3);               // Prints "BUSTED" on display and renders screen

                    CLEAR_LEDS;                                                 // Clears LEDs
                    setBit(PORTD, PD5);                                         // Lights up RED LED

                    setBit(PORTC, PC3);                                         // Activates buzzer for 2 seconds
                    delayMs(2000);                                              // ...
                    clrBit(PORTC, PC3);                                         // ...

                    systemFlags.hasLockedIndicatorShown = true;
                }
            }
            if(systemFlags.buttonPressed) {                                     // User has pressed unlock button
                if(!systemFlags.hasCheckedAlcoholLevel) {
                    delayMs(10);                                                // 10ms debounce

                    printf("BUTTON PRESSED - CHECKING ALCOHOL LEVEL\r\n");

                    adc.startConversion();                                      // Updates adcValue
                    adc.waitUntilConversionFinish();
                }

                if(adcValue <= ALCOHOL_THRESHOLD) {                             // Only unlocks if alcohol level has stabilized
                    timer1.setClockSource(Timer1::ClockSource::PRESCALER_256);  // Enables TIMER1 Clock

                    systemFlags.systemLocked = false;
                    systemFlags.hasLockedIndicatorShown = false;
                    statesMachine = StatesMachine::CLEAR;                       // Jumps to CLEAR state
                }

                if(adcValue > ALCOHOL_THRESHOLD &&
                        !systemFlags.hasCheckedAlcoholLevel) {     // Doesn't unlock if alcohol level hasn't stabilized
                    printf("ALCOHOL LEVEL TOO HIGH: %u\r\n", adcValue);         // and runs only once every button press

                    char bufferStatus[7] = "BUSTED";
                    handleUpdateStatusOnDisplay(bufferStatus, 3);               // Prints "BUSTED" on display and renders screen

                    systemFlags.hasCheckedAlcoholLevel = true;
                }
            }
            break;
        case StatesMachine::ERROR:
            char bufferStatus[6] = "ERROR";
            handleUpdateStatusOnDisplay(bufferStatus, 5);                       // Prints "ERROR" on display and renders screen

            CLEAR_LEDS;                                                         // Clears LEDs
            for(uint8_t i = 0; i < static_cast<uint8_t>(errorCode); i++) {      // Blinks BLUE LED as Error Code number
                setBit(PORTD, PD7);
                delayMs(300);
                clrBit(PORTD, PD7);
                delayMs(300);
            }
            delayMs(2000);
            break;
        }

    }

    return 0;
}

void setError(ErrorCode code)
{
    systemFlags.error = true;
    errorCode = code;
}

void adcConversionCompleteCallback()
{
    adcValue = ADC;
    systemFlags.adcReady = true;
}

void pcint2InterruptCallback()
{
    if(isBitClr(PIND, PIND4)) {                                                 // Unlock push button has been pressed
        systemFlags.buttonPressed = true;
        systemFlags.hasCheckedAlcoholLevel = false;
    }
}

void handlePrintTemperature()
{
    uint8_t temp[2] = {0};

    // Leitura do registrador de temperatura (0x00), 2 bytes
    if(twi.readReg(TEMP_REG, temp, 2)) {
        printf("Temp: %02u.%01u C\r\n", temp[0], temp[1]);
        tempValue[0] = temp[0];                         // Updates temperature value on global variable
        tempValue[1] = temp[1] / 100;                   // Limits to 1 decimal
    } else {
        printf("Erro na leitura da temperatura\r\n");
        setError(ErrorCode::TEMP_READ);
    }
}

void handleValidateAlcoholLevel()
{
    if(adcValue > ALCOHOL_THRESHOLD) {
        systemFlags.systemLocked = true;
    }
}

void handleValidateTemperatureLevel()
{
    if(tempValue[0] >= TEMP_THRESHOLD) {
        systemFlags.isTemperatureHigh = true;
    } else {
        systemFlags.isTemperatureHigh = false;
    }
}

void handleResetDisplay()
{
    char title[17] = "DRUNKBUSTER V1.0";                // Constant text that is shown on display no matter the system state
    char alcoholTitle[8] = "Alcohol";                   // ...
    char temperatureTitle[5] = "Temp";                  // ...
    char bufferAdc[20];                                 // ...
    char bufferTemp[20];                                // ...

    sprintf(bufferAdc, "%04u", adcValue);               // Transforms ADC value (int alcohol level) to string
    sprintf(bufferTemp, "%02u.%01u",                    // Transforms temperature value (int alcohol level) to string
            tempValue[0],
            tempValue[1]
    );

    display.clearScreen();                              // Clears screen
    display.setFont((uint8_t *)TinyFont);               // Sets font to Tiny

    display.print(title, 0, 0);                         // Updates title on screen
    display.print(alcoholTitle, 0, 8);                  // Updates alcoholTitle on screen
    display.print(temperatureTitle, 40, 8);             // Updates temperatureTitle on screen

    display.setFont((uint8_t *)SmallFont);              // Sets font to Small

    display.print(bufferAdc, 0, 16);                    // Updates alcohol value on screen
    display.print(bufferTemp, 40, 16);                  // Updates temperature value on screen

    display.drawRectangle(0, 32, 40, 43);               // Draws state status rectangle
    display.drawRectangle(46, 32, 80, 43);              // Draws temperature status rectangle
}

void handleUpdateStatusOnDisplay(char *bufferStatus, uint8_t x, bool_t useRenderScreen)
{
    handleResetDisplay();
    display.print(bufferStatus, x, 35);

    if(useRenderScreen) {
        display.renderScreen();
    }
}
