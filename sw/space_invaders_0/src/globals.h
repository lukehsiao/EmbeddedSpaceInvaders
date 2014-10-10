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

// Starting location in DDR where we will store the images that we display.
#define FRAME_BUFFER_0_ADDR 0xC1000000  

// We defined this to make it easy to see how many bits are used.
// Throughout our code, we tried to used the smallest amount of bits for
// what we needed in the global variables.
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int

// Color Definitions
#define GREEN 0x0000FF00
#define WHITE 0x00FFFFFF
#define RED   0x00FF0000
#define BLACK 0x00000000

// Movement Definitions
#define X_SHIFT 6           // # of pixels the Alien Block shifts horizontally.
#define TANK_BULLET_SPEED 15    // # of pixel the tank bullet moves per update
#define ALIEN_BULLET_SPEED 10   // # of pixel the alien bullets moves per update
#define ALIEN_BULLET_WIDTH 6
#define ALIEN_BULLET_HEIGHT 10
#define TANK_BULLET_WIDTH 2
#define TANK_BULLET_HEIGHT 10

#include "bitmaps.h"

typedef struct {u32 x; u32 y;} point_t;
typedef struct {point_t position; u8 type; u8 guise;} alienBullet;


/////////////////////////////////////
// Initialize the program
/////////////////////////////////////
void initGlobals();

/////////////////////////////////////
// Setup the Tank Globals
/////////////////////////////////////
void setTankPositionGlobal(unsigned short val);	// they can only modify x
point_t getTankPositionGlobal();

void setTankBulletPosition(point_t val);
point_t getTankBulletPosition();

void setTankLife(u8 newVal);
u8 getTankLife();


/////////////////////////////////////
// Setup the Alien Globals
/////////////////////////////////////
void setAlienBlockPosition(point_t val);
point_t getAlienBlockPosition();

void setAlienDirection(u8 tempDirection);
u8 getAlienDirection();

void setAlienStatus(u32 alienNumber, u32 status);
u16 getAlienStatus(u32 alienNumber);

u8 toggleAlienGuise();
u8 getAlienGuise();

alienBullet getAlienBullet(u8 bulletNum);
void setAlienBullet(alienBullet val, u8 bulletNum);

u16 getRightPad();
u16 getLeftPad();

u8 setLeftCol(u8 leftCol);
u8 setRightCol(u8 rightCol);
u8 getLeftCol();
u8 getRightCol();



/////////////////////////////////////
// Setup the Bunker Globals
/////////////////////////////////////
void initBunkers();

void setBlockState(u8 bunkerNumber, u8 blockNumber, u8 erosion);
u8 getBlockState(u8 bunkerNumber, u8 blockNumber);
u32 getBunkerState(u8 bunkerNumber);
point_t getBunkerPosition(u8 bunkerNumber);


/////////////////////////////////////
// Setup the Score/Lives Globals
/////////////////////////////////////
void setScore(u32 newScore);
u32 getScore();

void setLives(u8 newLives);
u8 getLives();

#endif
