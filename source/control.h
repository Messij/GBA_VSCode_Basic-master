#ifndef CONTROL_H_
#define CONTROL_H_

#include "main.h"

#define KEY_A            1
#define KEY_B            2
#define KEY_SELECT       4
#define KEY_START        8
#define KEY_RIGHT        16
#define KEY_LEFT         32
#define KEY_UP           64
#define KEY_DOWN         128
#define KEY_R            256
#define KEY_L            512
#define KEY_NONE         1023

#define KEY_PRESSED *KEYS != KEY_NONE

#define A       !(*KEYS & KEY_A)
#define B       !(*KEYS & KEY_B)
#define SELECT  !(*KEYS & KEY_SELECT)
#define START   !(*KEYS & KEY_START)
#define RIGHT   !(*KEYS & KEY_RIGHT)
#define LEFT    !(*KEYS & KEY_LEFT)
#define UP      !(*KEYS & KEY_UP)
#define DOWN    !(*KEYS & KEY_DOWN)
#define R       !(*KEYS & KEY_R)
#define L       !(*KEYS & KEY_L)

void GetInput();
void WaitKey(int key);

#endif