#include "control.h"

volatile u32* KEYS = (volatile u32*) 0x04000130;

void GetInput()
{
	if (!(*KEYS & KEY_DOWN))
	{

	}
	if (!(*KEYS & KEY_UP))
	{

	}
	if (!(*KEYS & KEY_RIGHT))
	{

	}
    if (!(*KEYS & KEY_LEFT))
	{

	}
}

void WaitKey(int key)
{
	while (1)
		if (!(*KEYS & key)) break;
}
