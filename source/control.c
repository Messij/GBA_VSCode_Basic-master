#include "control.h"

volatile u32* KEYS = (volatile u32*) 0x04000130;

void GetInput(u32* KEYS)
{
	if (!(*KEYS & DOWN))
	{

	}
	if (!(*KEYS & UP))
	{

	}
	if (!(*KEYS & RIGHT))
	{

	}
    if (!(*KEYS & LEFT))
	{

	}
}

void WaitKey(int key)
{
	while (1)
		if (!(*KEYS & key)) break;
}
