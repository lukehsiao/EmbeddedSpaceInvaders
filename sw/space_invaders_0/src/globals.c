/**
 * This implements globals.h
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "globals.h"

typedef struct {point_t position; u8 type;} alienBullet;

//******************* Variables of Space Invaders*************************
// Alien Variables
u16 alienStatus[5];		// one u16 tracks life/death of one row of aliens
point_t alienBlockPosition;  // Top-left position of the alien block.
alienBullet alienBullet_0;
alienBullet alienBullet_1;
alienBullet alienBullet_2;
alienBullet alienBullet_3;

// Bunker Variables
point_t bunkerPosition_0;	// Top-left position of the bunker.
point_t bunkerPosition_1;
point_t bunkerPosition_2;
point_t bunkerPosition_3;
u32 bunkerState_0;			// We track the 5 states of each of the 10 blocks
u32 bunkerState_1;			// using 3 bits of the u32.
u32 bunkerState_2;
u32 bunkerState_3;

// Tank Variables
point_t tankPosition;
point_t tankBulletPosition;

// Score & Lives Variables
u32 score;
u8 lives;	//starts w/ 3 and decrements each death
//******************* End Variables of Space Invaders**********************


/////////////////////////////////////
// Implement the Tank Globals
/////////////////////////////////////
void setTankPositionGlobal(unsigned short val) {
	tankPosition.x = val;
}
unsigned short getTankPositionGlobal() {
	return tankPosition.x;
}

void setTankBulletPosition(point_t val) {
	tankBulletPosition = val;
}
point_t getTankBulletPosition() {
	return tankBulletPosition;
}

/////////////////////////////////////
// Setup the Alien Globals
/////////////////////////////////////
void setAlienBlockPosition(point_t val) {
	alienBlockPosition = val;
}
point_t getAlienBlockPosition() {
	return alienBlockPosition;
}

void setAlienBulletPosition_0(point_t val, u8 bullet_type) {
	alienBullet_0.position = val;
	alienBullet_0.type = bullet_type;
}
point_t getAlienBulletPosition_0() {
	return alienBullet_0.position;
}
u8 getAlienBulletType_0() {
	return alienBullet_0.type;
}

void setAlienBulletPosition_1(point_t val, u8 bullet_type) {
	alienBullet_1.position = val;
	alienBullet_1.type = bullet_type;
}
point_t getAlienBulletPosition_1() {
	return alienBullet_1.position;
}
u8 getAlienBulletType_1() {
	return alienBullet_1.type;
}

void setAlienBulletPosition_2(point_t val, u8 bullet_type) {
	alienBullet_2.position = val;
	alienBullet_2.type = bullet_type;
}
point_t getAlienBulletPosition_2() {
	return alienBullet_2.position;
}
u8 getAlienBulletType_2() {
	return alienBullet_2.type;
}

void setAlienBulletPosition_3(point_t val, u8 bullet_type) {
	alienBullet_3.position = val;
	alienBullet_3.type = bullet_type;
}
point_t getAlienBulletPosition_3() {
	return alienBullet_3.position;
}
u8 getAlienBulletType_3() {
	return alienBullet_3.type;
}

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
