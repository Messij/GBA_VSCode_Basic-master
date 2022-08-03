#ifndef SPRITE_H
#define SPRITE_H

typedef unsigned short   u16;

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

#endif