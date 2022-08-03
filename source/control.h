#ifndef CONTROL_H_
#define CONTROL_H_

#include "main.h"

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

void GetInput();
void WaitKey(int key);

#endif