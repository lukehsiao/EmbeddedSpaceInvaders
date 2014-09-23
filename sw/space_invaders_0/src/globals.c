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
u32 bunkerState[4];			// We track the 5 states of each of the 10 blocks
							// using 3 bits of the u32.

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
	tankPosition.y = 440;
}
point_t getTankPositionGlobal() {
	return tankPosition;
}

void setTankBulletPosition(point_t val) {
	tankBulletPosition = val;
}
point_t getTankBulletPosition() {
	return tankBulletPosition;
}

/**
 * @ return 0xFF if it's not inside, 0 if it is
 */
u8 isInsideTank(point_t val) {
	return 0xFF;
}

/////////////////////////////////////
// Implement the Alien Globals
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
// Implement the Bunker Globals
/////////////////////////////////////

/**
 * Sets the erosion state of the specified block.
 * TODO: This may be a critical section!
 *
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 * @return the erosion state of the block specified
 */
void setBunkerState(u8 bunkerNumber, u8 blockNumber, u8 erosion) {
	u32 tempState = bunkerState[bunkerNumber];
	tempState = 0x3FFFFFFF & tempState;
	//clear old state by creating a ..1100011... mask and ANDing
	u32 mask = 0xFFFFFFF8; //...1111 1000
	u32 oneFill = 0x3FFFFFFF >> (30 - (blockNumber*3));
	mask = (mask << (blockNumber*3)) | oneFill;
	tempState = tempState & mask;

	//Set the new state
	u32 newErosion = (u32)erosion; // this will pad the left with 0s
	newErosion = newErosion << (blockNumber*3);
	tempState = tempState | newErosion;
	//xil_printf("\n\rTemp State is:\n\r%x", tempState);
	bunkerState[bunkerNumber] = tempState;
}

/**
 * Masks the u32 bunker state variable to return the
 * erosion state of the bunker block.
 *
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 * @return the erosion state of the block specified
 */
u8 getBunkerState(u8 bunkerNumber, u8 blockNumber) {
	u32 tempState = bunkerState[bunkerNumber];
	//Shift and mask only the 3 bits representing the state of the block
	u8 erosionState = (u8)(tempState >> (blockNumber*3));
	erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	return erosionState;
}

/////////////////////////////////////
// Implement the Score/Lives Globals
/////////////////////////////////////
void setScore(u32 newScore) {
	score = newScore;
}
u32 getScore() {
	return score;
}

void setLives(u8 newLives) {
	lives = newLives;
}
u8 getLives() {
	return lives;
}
