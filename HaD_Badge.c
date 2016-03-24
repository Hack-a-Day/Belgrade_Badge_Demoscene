#include <stdio.h>
#include "HaD_Badge.h"
#include "bh-badge-animate.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

uint8_t Buffer[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

SDL_Event event;

//SDL2 variables
void* nullptr;
SDL_Window *win;
SDL_Renderer *ren;

//These display size values are specific to the SDL2 emulator
#define PIXELRADIUS     8      
#define PIXELSPACING    5

//These color values are specific to the SDL2 emulator
//Color definitions
#define BLUE    0
#define YELLOW  1
#define RED     2
#define PINK    3
#define ORANGE  4
#define CYAN    5
#define BLACK   6
#define GREY    7
#define WHITE   8
#define LAVENDAR  9
#define GREEN   10

//Color values
static const uint8_t colors[][3] = {
    { 0, 0, 255 },      //Blue
    { 255, 255, 0 },    //Yellow
    { 255, 0, 0 },      //Red
    { 255, 153, 204 },  //Pink
    { 255, 102, 0 },    //Orange
    { 0, 255, 255 },    //Cyan
    { 0, 0, 0 },        //Black
    { 64, 64, 64 },     //Grey
    { 255, 255, 255 },  //White
    { 196, 64, 255},    //Lavendar
    { 0, 255, 0}        //Green
};

void initDisplay(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
    }

    win = SDL_CreateWindow(
        "sdlTest",
        100,
        100,
        (TOTPIXELX*(PIXELSPACING+PIXELRADIUS+PIXELRADIUS))+PIXELSPACING,
        (TOTPIXELY*(PIXELSPACING+PIXELRADIUS+PIXELRADIUS))+PIXELSPACING,
        SDL_WINDOW_SHOWN);

    if (win == nullptr) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    }

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    }

    displayClear();
    
}

void displayClear(void) {

    SDL_SetRenderDrawColor(ren, colors[BLACK][0], colors[BLACK][1], colors[BLACK][2], 255);
    SDL_RenderClear(ren);
    
    //Turn all LEDs to off state
    for (uint8_t i=0; i<TOTPIXELX; i++) {
        for (uint8_t j=0; j<TOTPIXELY; j++) {
            displayPixel(i,j,OFF);
        }
    }
}

void displayPixel(uint8_t x, uint8_t y, uint8_t state) {
    if (state) { Buffer[y] |= 1<<7-x; }
    else { Buffer[y] &= ~(1<<7-x); }
}

void showSDLpixel(uint8_t x, uint8_t y, uint8_t state){
    uint8_t color = GREY;
    if (state) { color = RED; }
    filledCircleRGBA(
        ren,
        (x*(PIXELRADIUS+PIXELRADIUS+PIXELSPACING))+PIXELRADIUS+PIXELSPACING,
        (y*(PIXELRADIUS+PIXELRADIUS+PIXELSPACING))+PIXELRADIUS+PIXELSPACING,
        PIXELRADIUS,
        colors[color][0],
        colors[color][1],
        colors[color][2],
        255
        );
}

void displayLatch(void) {
    //In hardware, this is taken care of in an interrupt
    //But is needed here for SDL to show our buffer
    for (uint8_t row=0; row<16; row++) {
        for (uint8_t col=0; col<8; col++) {
            if (Buffer[row] & 1<<7-col) { showSDLpixel(col, row, ON); }
                else { showSDLpixel(col, row, OFF); }
        }
    }
    SDL_RenderPresent(ren);
}

void displayClose(void) {
    SDL_DestroyWindow(win);
    SDL_Quit();
}

uint8_t getControl(void) {
    if (SDL_PollEvent(&event)) {
        /*
        if (event.type == SDL_QUIT) {
            gameRunning = 0;
        }
        */
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode keyPressed = event.key.keysym.sym;

            switch (keyPressed)
            {
                case SDLK_ESCAPE:
                    return ESCAPE;
                case SDLK_UP:
                    return UP;
                case SDLK_DOWN:
                    return DOWN;
                case SDLK_LEFT:
                    return LEFT;
                case SDLK_RIGHT:
                    return RIGHT;
                case SDLK_SPACE:
                    return BUTTON;
            }
        }
    }
    return NOINPUT;
}

void initTime(void) {
    //Not needed for SDL2 emulator but will be for actual hardware
}

uint32_t getTime(void) {
    return SDL_GetTicks();
}

void controlDelayMs(uint16_t ms) {
    SDL_Delay(ms);
}

int main(void) {
    initDisplay();
    initTime();
    displayLatch();
    
    animateBadge();
    return 0;
}
