# Hackaday Belgrade Badge Demoscene

This is a barebones C framework for the Hackaday Belgrade Badge
which can be used to write demoscene entries (even if you are
not going to the conference).

All of your code should be written in two files:
* bh-badge-animate.c
* bh-badge-animate.h

The rest of the files should be unchanged. This ensures that code
which works in the emulator will work on the badge. Your demo must
contain a function called `void animateBadge(void);` which loops
forever.

In it's current state, this demo puts a single pixel on the screen 
(the ball) and allows the user to move it around with the arrow keys
without leaving the edges of the screen.

## Software Emulator

If you do not have the hardware on hand, there is a software emulator build using SDL2 which will show the badge's 8x16 LED matrix and emulator four buttons (up, down, left, right).

Install the SDL2 and SDL2_gfx libraries:

```sudo apt-get install libsdl2-dev sudo apt-get install libsdl2-gfx-dev``` 

Launch the emulator by typing `make`

Controls:

* UP
* DOWN
* LEFT
* RIGHT
* ESCAPE (closes SDL2 emulator, not use in hardware badge)

### Emulator under OSX

Small Room Labs posted simple [instructions to get this working on OSX](https://github.com/Hack-a-Day/Belgrade_Badge_Demoscene/issues/3)

## The Badge API (Straight from HaD_Badge.h)
```
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
```
## Gotchas

`displayLatch()` is **very** important and kind of weird. On the emulator you must
call this function or your changes to the display will not appear. However, on the
hardware your changes will appear as soon as you've made them (using `displayPixel(x,y,state)`).
So, when writing for the emulator remember to add displayLatch when done making changes.
Leaving the code in there will not affect the badge, but you can't make a change and assume
that waiting to latch the change will work on the hardware.

I included `controlDelayMs(ms)` because sometimes you just want to wait a few milliseconds
before doing anything. This is a blocking function. It won't prevent the display from scanning
or the buttons from being read, but your code won't react to button presse while a blocking
delay is being used. `getTime()` is a better method... get the time in milliseconds, store it in a
variable and increment by the amount you want to delay, then poll the function for that time to pass.

`Buffer[16]` does not work the way you think it works. The buffer is formatted for easy scanning
on the hardware badge. So the x and y coordinates map differently that you would think. Many people
don't need to know this, but if you choose to write your own functions that can directly alter the
`Buffer` (in [my own demo](https://github.com/szczys/belgrade-hackaday-badge) I wrote a framebuffer that does this) you will need to dig into the code
and figure out what you're doing or else the badge will behave strangely when we run your demo.

## Not Implemented
The official badge firmware is written in assembly. This C port modelled after that code but doesn't use any of that assembly and doesn't implement some of it.

Here are some badge features **not** implemented in this C framework:

* INT button (fifth button found on hardware badge)
* Pause and Sleep (toggled on badge by INT button)
* IR communications 
