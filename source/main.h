#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

#define DMA_ENABLE                    0x80000000
#define DMA_INTERUPT_ENABLE           0x40000000
#define DMA_TIMEING_IMMEDIATE         0x00000000
#define DMA_TIMEING_VBLANK            0x10000000
#define DMA_TIMEING_HBLANK            0x20000000
#define DMA_TIMEING_SYNC_TO_DISPLAY   0x30000000
#define DMA_16                        0x00000000
#define DMA_32                        0x04000000
#define DMA_REPEATE                   0x02000000
#define DMA_SOURCE_INCREMENT          0x00000000
#define DMA_SOURCE_DECREMENT          0x00800000
#define DMA_SOURCE_FIXED              0x01000000
#define DMA_DEST_INCREMENT            0x00000000
#define DMA_DEST_DECREMENT            0x00200000
#define DMA_DEST_FIXED                0x00400000
#define DMA_DEST_RELOAD               0x00600000

#define DMA_32NOW   DMA_ENABLE | DMA_TIMEING_IMMEDIATE | DMA_32 // Immediate copy 32 bits 
#define DMA_16NOW   DMA_ENABLE | DMA_TIMEING_IMMEDIATE | DMA_16 // Immediate copy 16 bits 

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

// Screen
#define REG_DISPCNT      *(u32*) 0x4000000 // mode ?

void WaitForVSync();
void WaitForSeconds(int n);

#endif