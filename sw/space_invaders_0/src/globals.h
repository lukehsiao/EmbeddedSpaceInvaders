/**
 * This interface will provide a structured way to handle globals
 * using set and get functions.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#ifndef globals_h
#define globals_h
#define FRAME_BUFFER_0_ADDR 0xC1000000  // Starting location in DDR where we will store the images that we display.

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define RED   0x00FF0000
#define BLACK 0x00000000
#define X_SHIFT 6
#define Y_SHIFT 6
#define TANK_BULLET_SPEED 15
#define ALIEN_BULLET_SPEED 10
#define ALIEN_BULLET_WIDTH 6
#define ALIEN_BULLET_HEIGHT 10
#define TANK_BULLET_WIDTH 2
#define TANK_BULLET_HEIGHT 10
#include "bitmaps.h"

//typedef unsigned long ulong;

typedef struct {u32 x; u32 y;} point_t;
typedef struct {point_t position; u8 type; u8 guise;} alienBullet;

void initGlobals();

/////////////////////////////////////
// Setup the Tank Globals
/////////////////////////////////////
void setTankPositionGlobal(unsigned short val);	// they can only modify x
point_t getTankPositionGlobal();

void setTankBulletPosition(point_t val);
point_t getTankBulletPosition();

u8 isInsideTank(point_t val);

/////////////////////////////////////
// Setup the Alien Globals
/////////////////////////////////////
void setAlienBlockPosition(point_t val);
point_t getAlienBlockPosition();
u16 getAlienStatus(u32 alienNumber);
void setAlienStatus(u32 alienNumber, u32 status);
point_t getAlienBulletPosition_0();
u8 getAlienBulletType_0();
u8 toggleAlienGuise();
u8 getAlienGuise();
u8 getAlienDirection();
void setAlienDirection(u8 tempDirection);

const u32* getAlienBulletArray(u8 bulletNum);
alienBullet getAlienBullet(u8 bulletNum);
void setAlienBullet(alienBullet val, u8 bulletNum);

u16 getRightPad();
u16 getLeftPad();
void setRightPad(u16 new);
void setLeftPad(u16 new);

/////////////////////////////////////
// Setup the Bunker Globals
/////////////////////////////////////
void initBunkers();
void setBlockState(u8 bunkerNumber, u8 blockNumber, u8 erosion);
u8 getBlockState(u8 bunkerNumber, u8 blockNumber);
u32 getBunkerState(u8 bunkerNumber);
point_t getBunkerPosition(u8 bunkerNumber);
u8 isInsideBunker(point_t val);

/////////////////////////////////////
// Setup the Score/Lives Globals
/////////////////////////////////////
void setScore(u32 newScore);
u32 getScore();

void setLives(u8 newLives);
u8 getLives();

#endif
