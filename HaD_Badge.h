#include <inttypes.h>

#define EMULATOR

#define TOTPIXELX       8
#define TOTPIXELY       16

extern uint8_t Buffer[16];

//LED Control definitions
#define OFF     0
#define ON      1

#ifdef __cplusplus
extern "C"{
#endif

//Directions of travel
#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3
//Miscellaneous
#define ESCAPE  4
#define NOINPUT 5
#define BUTTON  6


/*---- Display Prototypes ----*/
void initDisplay(void);             //Turn on display and set all LEDs off
void displayClear(void);            //Turn all LEDs off
void displayPixel(uint8_t x, uint8_t y, uint8_t state); //Set LED to state (ON|OFF)
void displayClose(void);            //Close the display (used for SDL2 emulator window)
void displayLatch(void);            //Make display changes visible (used for SDL2 emulator)
/*--------------------*/



/*---- Control Prototypes ----*/
void initControl(void);             //Setup button input
uint8_t getControl(void);           //Return last pressed button
void initTime(void);                //Initialize timekeeping hardware
uint32_t getTime(void);             //Return milliseconds (upcounting)
void controlDelayMs(uint16_t ms);   //Delay milliseconds (blocking)

#ifdef __cplusplus
} // extern "C"
#endif
