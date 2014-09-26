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
#include "bitmaps.h"

typedef struct {u16 x; u16 y;} point_t;

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

void setAlienBulletPosition_0(point_t val, u8 bullet_type);
point_t getAlienBulletPosition_0();
u8 getAlienBulletType_0();

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
void setBunkerState(u8 bunkerNumber, u8 blockNumber, u8 erosion);
u8 getBunkerState(u8 bunkerNumber, u8 blockNumber);
u8 isInsideBunker(point_t val);

/////////////////////////////////////
// Setup the Score/Lives Globals
/////////////////////////////////////
void setScore(u32 newScore);
u32 getScore();

void setLives(u8 newLives);
u8 getLives();

#endif
