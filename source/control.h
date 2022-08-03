#ifndef CONTROL_H
#define CONTROL_H

#include "main.h"

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

#endif