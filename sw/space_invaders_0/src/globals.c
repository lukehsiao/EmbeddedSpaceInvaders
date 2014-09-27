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
u8 alienGuise;
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
point_t tankBulletPosition; // Tank bullet is 2*8 pixels tall

// Score & Lives Variables
u32 score;
u8 lives;	//starts w/ 3 and decrements each death
//******************* End Variables of Space Invaders**********************

/////////////////////////////////////
// Initializations
/////////////////////////////////////
void initGlobals(){
	setTankPositionGlobal(200);
    initBunkers();
    alienGuise = 1;
	int i;
	for(i = 0; i < 5; i++) {
		alienStatus[i] = 0xFFFF;
	}

	point_t temp;
	temp.x = 150;
	temp.y = 50;
	setAlienBlockPosition(temp);
}

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

u8 toggleAlienGuise() {
	u8 temp = alienGuise;
	alienGuise = (~alienGuise) & 0x1;
	return temp;
}

u8 getAlienGuise() {
	return alienGuise;
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

void setAlienStatus(u32 alienNumber, u32 status) {
	if (alienNumber < 11) {
		// clearing the bit
		u32 bitTemp = alienStatus[0] & ~(0x1 << (15 - alienNumber));
		// Setting the bit
		alienStatus[0] = bitTemp | (status << (15 - alienNumber));
	}
	else if (alienNumber < 22) {
		// clearing the bit
		u32 bitTemp = alienStatus[1] & ~(0x1 << (15 - (alienNumber % 11)));
		// Setting the bit
		alienStatus[1] = bitTemp | (status << (15 - (alienNumber % 11)));
	}
	else if (alienNumber < 33) {
		// clearing the bit
		u32 bitTemp = alienStatus[2] & ~(0x1 << (15 - (alienNumber % 11)));
		// Setting the bit
		alienStatus[2] = bitTemp | (status << (15 - (alienNumber % 11)));
	}
	else if (alienNumber < 44) {
		// clearing the bit
		u32 bitTemp = alienStatus[3] & ~(0x1 << (15 - (alienNumber % 11)));
		// Setting the bit
		alienStatus[3] = bitTemp | (status << (15 - (alienNumber % 11)));
	}
	else {
		// clearing the bit
		u32 bitTemp = alienStatus[4] & ~(0x1 << (15 - (alienNumber % 11)));
		// Setting the bit
		alienStatus[4] = bitTemp | (status << (15 - (alienNumber % 11)));
	}
}

u16 getAlienStatus(u32 alienNumber) {
	u16 temp;
	if (alienNumber < 11) {
		temp = (alienStatus[0] >> (15-alienNumber)) & 0x1;
	}
	else if (alienNumber < 22) {
		temp = (alienStatus[1] >> (15-(alienNumber % 11))) & 0x1;
	}
	else if (alienNumber < 33) {
		temp = (alienStatus[2] >> (15-(alienNumber % 11))) & 0x1;
	}
	else if (alienNumber < 44) {
		temp = (alienStatus[3] >> (15-(alienNumber % 11))) & 0x1;
	}
	else {
		temp = (alienStatus[4] >> (15-(alienNumber % 11))) & 0x1;
	}
	return temp;
}

/////////////////////////////////////
// Implement the Bunker Globals
/////////////////////////////////////
void initBunkers() {
	int i;
	for (i = 0; i < 4; i++) {
		bunkerState[i] = 0x0000004F;
	}
	bunkerPosition_0.x = 104;
	bunkerPosition_0.y = 380;

	bunkerPosition_1.x = 232;
	bunkerPosition_1.y = 380;

	bunkerPosition_2.x = 360;
	bunkerPosition_2.y = 380;

	bunkerPosition_3.x = 488;
	bunkerPosition_3.y = 380;
}
point_t getBunkerPosition(u8 bunkerNumber) {
	point_t temp;
	switch(bunkerNumber) {
		case 0:
			return bunkerPosition_0;
		case 1:
			return bunkerPosition_1;
		case 2:
			return bunkerPosition_2;
		case 3:
			return bunkerPosition_3;
		default:
			temp.x = 0;
			temp.y = 0;
			return temp;
	}
}

/* Gets the bunker row for the specified bunker and block
 *
 * @param row
 * @param col
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 *
*/
u32 getBunkerRow(u32 row, u32 col, u8 bunkerNumber, u8 blockNumber) {
	u32 temp;
	switch(bunkerNumber) {
		case 0:
			temp = row - bunkerPosition_0.y;
			break;
		case 1:
			temp = row - bunkerPosition_1.y;
			break;
		case 2:
			temp = row - bunkerPosition_2.y;
			break;
		case 3:
			temp = row - bunkerPosition_3.y;
			break;
		default:
			//xil_printf("\n\r\tWrong BunkerNumber in getBunkerRow");
			break;
	}
	return temp;
}

/* Gets the bunker col for the specified bunker and block
 *
 * @param row
 * @param col
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 *
*/
u32 getBunkerCol(u32 row, u32 col, u8 bunkerNumber, u8 blockNumber) {
	u32 temp;
	switch(bunkerNumber) {
		case 0:
			temp = col - bunkerPosition_0.x;
			break;
		case 1:
			temp = col - bunkerPosition_1.x;
			break;
		case 2:
			temp = col - bunkerPosition_2.x;
			break;
		case 3:
			temp = col - bunkerPosition_3.x;
			break;
		default:
			//xil_printf("\n\r\tWrong BunkerNumber in getBunkerCol\n\r");
			break;
	}
	return temp;
}
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
	u32 newErosion;
	if (blockNumber < 9) {
		//Set the new state
		newErosion = (u32)erosion; // this will pad the left with 0s
		newErosion = newErosion << (blockNumber*3);
		tempState = tempState | newErosion;
	}
	else if (blockNumber == 9 || blockNumber == 10) {
		return;
	}
	else if (blockNumber == 11) {
		//Set the new state
		newErosion = (u32)erosion; // this will pad the left with 0s
		newErosion = newErosion << (27);
		tempState = tempState | newErosion;
	}
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
u8 getBlockState(u8 bunkerNumber, u8 blockNumber) {
	u32 tempState = bunkerState[bunkerNumber];
	u8 erosionState;
	if (blockNumber < 9) {
		//Shift and mask only the 3 bits representing the state of the block
		erosionState = (u8)(tempState >> (blockNumber*3));
		erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	}
	else if (blockNumber == 9 || blockNumber == 10) {
		erosionState = 4;	// return a black block
	}
	else if (blockNumber == 11) {
		erosionState = (u8)(tempState >> (27));
		erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	}
	return erosionState;
}

u32 getBunkerState(u8 bunkerNumber) {
	return bunkerState[bunkerNumber];
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
