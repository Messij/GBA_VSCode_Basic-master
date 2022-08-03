#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "color.h"

// types usuels sur GBA
typedef unsigned char    u8;
typedef unsigned short   u16;
typedef unsigned long    u32;

typedef signed char      s8;
typedef signed short     s16;
typedef signed long      s32;

// dimensions de l'ecran
#define SCREEN_WIDTH     	240
#define SCREEN_HEIGHT    	160
#define SCREEN_HALF_W    	SCREEN_WIDTH / 2
#define SCREEN_HALF_H    	SCREEN_HEIGHT / 2
#define SCREEN_RIGHT		SCREEN_WIDTH - 1
#define SCREEN_LEFT			0
#define SCREEN_UP			0
#define SCREEN_DOWN			SCREEN_HEIGHT - 1

// Double buffering
#define BACKBUFFER 			0x10
#define videoFrontBuffer 	(u16*) 0x6000000
#define videoBackBuffer 	(u16*) 0x600A000

// Screen Video Mode
#define MODE_0           0x0
#define MODE_1           0x1
#define MODE_2           0x2
#define MODE_3           0x3
#define MODE_4           0x4
#define MODE_5           0x5

// DMA Direct Memory Access
#define REG_DMA3CNT *(u32*) 0x40000DC // Commade Register
#define REG_DMA3DAD *(u32*) 0x40000D8 // Data Adress Destination
#define REG_DMA3SAD *(u32*) 0x40000D4 // Source Adress Destination
#define DMA_32NOW DMA_ENABLE | DMA_TIMEING_IMMEDIATE | DMA_32 // Immediate copy 32 bits 
#define DMA_16NOW DMA_ENABLE | DMA_TIMEING_IMMEDIATE | DMA_16 // Immediate copy 16 bits 

// Register
#define REG_TM2D         *(volatile u16*) 0x4000108
#define REG_TM2CNT       *(volatile u16*) 0x400010A
#define REG_TM3D         *(volatile u16*) 0x400010C // timer value
#define REG_TM3CNT       *(volatile u16*) 0x400010E // timer controle register

// Timer
#define TIMER_FREQUENCY_SYSTEM 0x0 // System clock frequency, 16.78 Mhz
#define TIMER_FREQUENCY_64 0x1 // Every 64 clock tick , every 3.814 ms
#define TIMER_FREQUENCY_256 0x2 // Every 256 clock tick , every  15.256 ms
#define TIMER_FREQUENCY_1024 0x3 // Every 1024 clock tick , every  61.025 ms
#define TIMER_OVERFLOW 0x4 // flow timer in the next when max value reach
#define TIMER_ENABLE 0x80
#define TIMER_IRQ_ENABLE 0x40

// Background
#define BG2              0x400; // background 2

// Screen adresses
u32* mode  = 	(u32*) 0x4000000;
u16* palette = 	(u16*) 0x5000000; // 256 valeurs sur 16 bits
u16* videoBuffer = (u16*) 0x6000000; // screen 240x160 pixel on 15 bits = 32765 colors

// Controls
volatile u32* KEYS = (volatile u32*) 0x04000130;
#define A            1
#define B            2
#define SELECT       4
#define START        8
#define RIGHT        16
#define LEFT         32
#define UP           64
#define DOWN         128
#define R            256
#define L            512

// Sprite
typedef struct tSprite
{
  u16 attribute0;
  u16 attribute1;
  u16 attribute2;
  u16 attribute3;
} Sprite;

#define MAX_SPRITES 128
Sprite OAMSpriteBuffer[MAX_SPRITES]; // OAM Buffer

// Drawing Functions
void DrawPixel(int x, int y, u16);
void DrawVerticalLine(int x, u16 color);
void DrawHorizontalLine(int y, u16 color);
void DrawDegraded();
void CreatePalette();
void DrawImage(const u16 data[], const u16 palette[]);
void FlipBuffers();
void WaitKey(int key);
void GetInput();
void WaitForVSync();
void WaitForSeconds(int n);
void TheRedDot(); // Draw a cntrolable red dot
void CopySpriteBuffer(); // OAM Buffer
