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
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define RED   0x00FF0000
#define BLACK 0x00000000
#include "bitmaps.h"

//typedef unsigned long ulong;

typedef struct {u32 x; u32 y;} point_t;

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
#define X_SHIFT 6
#define Y_SHIFT 6
void setAlienBlockPosition(point_t val);
point_t getAlienBlockPosition();
u16 getAlienStatus(u32 alienNumber);
void setAlienStatus(u32 alienNumber, u32 status);
void setAlienBulletPosition_0(point_t val, u8 bullet_type);
point_t getAlienBulletPosition_0();
u8 getAlienBulletType_0();
u8 toggleAlienGuise();
u8 getAlienGuise();
u8 getAlienDirection();
void setAlienDirection(u8 tempDirection);

void setAlienBulletPosition_1(point_t val, u8 bullet_type);
point_t getAlienBulletPosition_1();
u8 getAlienBulletType_1();

void setAlienBulletPosition_2(point_t val, u8 bullet_type);
point_t getAlienBulletPosition_2();
u8 getAlienBulletType_2();

void setAlienBulletPosition_3(point_t val, u8 bullet_type);
point_t getAlienBulletPosition_3();
u8 getAlienBulletType_3();

/////////////////////////////////////
// Setup the Bunker Globals
/////////////////////////////////////
void initBunkers();
void setBunkerState(u8 bunkerNumber, u8 blockNumber, u8 erosion);
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
