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

typedef struct {unsigned short x; unsigned short y;} point_t;

/////////////////////////////////////
// Setup the Tank Globals
/////////////////////////////////////
void setTankPositionGlobal(unsigned short val);	// they can only modify x
unsigned short getTankPositionGlobal();

void setTankBulletPosition(point_t val);
point_t getTankBulletPosition();

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
void setBunkerState_0(u8 erosion_state);
u8 getBunkerState_0();

void setBunkerState_1(u8 erosion_state);
u8 getBunkerState_1();

void setBunkerState_2(u8 erosion_state);
u8 getBunkerState_2();

void setBunkerState_3(u8 erosion_state);
u8 getBunkerState_3();

#endif
