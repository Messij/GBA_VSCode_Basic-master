#ifndef DRAW_H
#define DRAW_H

#include "main.h"

void DrawPixel(int x, int y, u16);
void DrawVerticalLine(int x, u16 color);
void DrawHorizontalLine(int y, u16 color);
void DrawDegraded();
void CreatePalette();
void DrawImage(const u16 data[], const u16 palette[]);
void FlipBuffers();
void TheRedDot(); // Draw a cntrolable red dot
void FadeOut();

#endif