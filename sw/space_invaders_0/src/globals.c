/**
 * This implements globals.h
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "globals.h"

u32* framePointer0 = (unsigned int *) FRAME_BUFFER_0_ADDR;
u32* framePointer1 = ((unsigned int *) FRAME_BUFFER_0_ADDR) + 640*480;

//******************* Variables of Space Invaders*************************
// Alien Variables
u16 alienStatus[5];		// one u16 tracks life/death of one row of aliens
point_t alienBlockPosition;  // Top-left position of the alien block.
u8 alienGuise;
alienBullet alienBullet_0;
alienBullet alienBullet_1;
alienBullet alienBullet_2;
alienBullet alienBullet_3;
u8 direction;	//1 = right, 0 = left

u16 rightPad;	//to allow for edge to edge rendering
u16 leftPad;
u8  leftMostColumn;		// left-most column number with aliens still alive
u8  rightMostColumn;	// right-most column number with aliens still alive

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
u8 tankLife;				// 1 = Tank Alive  0 = Tank Dead

// Score & Lives Variables
u32 score;
u8 lives;	//starts w/ 3 and decrements each death
//******************* End Variables of Space Invaders**********************

/////////////////////////////////////
// Initializations
/////////////////////////////////////
void initGlobals(){
	// Initialize Tank
	setTankPositionGlobal(200);


	// Initialize Bunkers
    initBunkers();

    // Initialize Aliens
    alienGuise = 1;
	int i;
	for(i = 0; i < 5; i++) {
		alienStatus[i] = 0xFFFF;
	}
	leftMostColumn = 0;
	rightMostColumn = 10;
	point_t temp;
	temp.x = 160;
	temp.y = 70;
	setAlienBlockPosition(temp);
	direction = 1;

	// Initialize Game Variables
	setLives(3);
	setScore(7348);


	alienBullet bullet;
	u8 j;
	for(j= 0; j < 4; j++){
		bullet = getAlienBullet(j);
		temp.x = 20 + 150*j;
		temp.y = 200 + 50*j;
		bullet.position = temp;
		bullet.type = j%2;
		setAlienBullet(bullet, j);
	}

	temp.y = 8888;
	temp.x = 20;
	setTankBulletPosition(temp);
	leftPad = 0;
	rightPad = 640;
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

void setTankLife(u8 newVal) {
	tankLife = newVal;
}

u8 getTankLife() {
	return tankLife;
}

/////////////////////////////////////
// Implement the Alien Globals
/////////////////////////////////////

/**
 * Sets the value of the top-left corner of the alien block
 * @param val The coordinates to set the alien block position to
 */
void setAlienBlockPosition(point_t val) {
	alienBlockPosition = val;
}

/**
 * @return The coordinates of the top-left corner of the block.
 */
point_t getAlienBlockPosition() {
	return alienBlockPosition;
}

/**
 * Sets the alien block to move in the passed in direction.
 * @param tempDirection The direction you want the block to move.
 */
void setAlienDirection(u8 tempDirection) {
	direction = tempDirection;
}

/**
 * @return The direction that the aliens are going.
 */
u8 getAlienDirection() {
	return direction;
}

/**
 * Sets the alien at alienNumber to the given status.
 * @param alienNumber Which alien to edit.
 * @param status The alien's status. 0 = dead, 1 = alive.
 */
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

/**
 * Read the alien status for the given alien.
 * @param alienNumber Which alien to get the status of.
 * @return 0 if dead, 1 if alive.
 */
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

/**
 * Toggles the alien guise from open to closed and closed to open.
 * @return The value that the alien guise used to be before toggling.
 */
u8 toggleAlienGuise() {
	u8 temp = alienGuise;
	alienGuise = (~alienGuise) & 0x1;
	return temp;
}

/**
 * @return The value of the alien guise.
 */
u8 getAlienGuise() {
	return alienGuise;
}

/**
 * Gets the specified alien bullet.
 * @param bulletNum The Alien Bullet struct associated with that number. (0-3)
 * @return the alien bullet associated with bulletNum
 */
alienBullet getAlienBullet(u8 bulletNum) {
	alienBullet bullet;
	switch (bulletNum)
	{
	case 0:
		bullet = alienBullet_0;
		break;
	case 1:
		bullet = alienBullet_1;
		break;
	case 2:
		bullet = alienBullet_2;
		break;
	default:
		bullet = alienBullet_3;
		break;
	}
	return bullet;
}

/**
 * Sets the specified alien bullet values.
 * @param val The alien bullet to set.
 * @param bulletNum the number to assign to the passed in alien bullet.
 */
void setAlienBullet(alienBullet val, u8 bulletNum) {
	alienBullet bullet;
	bullet = getAlienBullet(bulletNum);
	switch (bulletNum)
	{
	case 0:
		alienBullet_0 = val;
		break;
	case 1:
		alienBullet_1 = val;
		break;
	case 2:
		alienBullet_2 = val;
	break;
	default:
		alienBullet_3 = val;
	break;
	}
}

/**
 * Sets the right padding value to newVal
 * @param newVal The value to set the right pad to.
 */
void setRightPad(u16 newVal) {
	rightPad = newVal;
}

/**
 * @return The value of the right pad.
 */
u16 getRightPad() {
	return rightPad;
}

/**
 * Sets the left padding value to newVal
 * @param newVal The value to set the left pad to.
 */
void setLeftPad(u16 newVal) {
	leftPad = newVal;
}

/**
 * @return The value of the left pad.
 */
u16 getLeftPad() {
	return leftPad;
}

/**
 * @return The value of the left pad.
 */
u8 getLeftCol() {
	return leftMostColumn;
}

/**
 * @return The value of the left pad.
 */
u8 getRightCol() {
	return rightMostColumn;
}

/**
 * Set the value of the left pad.
 */
void setLeftCol(u8 leftCol) {
	leftMostColumn = leftCol;

	//Adjust the border
	leftPad = 0 + (32*leftMostColumn);

}

/**
 * set the value of the left pad.
 */
void setRightCol(u8 rightCol) {
	rightMostColumn = rightCol;

	//Adjust the border
	rightPad = 640 + (32*(10-rightMostColumn));
}

/////////////////////////////////////
// Implement the Bunker Globals
/////////////////////////////////////

/**
 * Initializes the bunkers in the correct positions
 */
void initBunkers() {
	int i;
	for (i = 0; i < 4; i++) {
		bunkerState[i] = 0x00000000;
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

/**
 * Sets the erosion state of the specified block.
 * TODO: This may be a critical section!
 *
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 * @return the erosion state of the block specified
 */
void setBlockState(u8 bunkerNumber, u8 blockNumber, u8 erosion) {
	u32 tempState = bunkerState[bunkerNumber];
	tempState = 0x3FFFFFFF & tempState;
	u32 newErosion;
	
	if (blockNumber < 9) {
		//clear old state by creating a ..1100011... mask and ANDing
		u32 mask = 0xFFFFFFF8; //...1111 1000
		u32 oneFill = 0x3FFFFFFF >> (30 - (blockNumber*3));
		mask = (mask << (blockNumber*3)) | oneFill;
		tempState = tempState & mask;

		//Set the new state
		newErosion = (u32)erosion; // this will pad the left with 0s
		newErosion = newErosion << (blockNumber*3);
		tempState = tempState | newErosion;
		bunkerState[bunkerNumber] = tempState;
	}
	else if (blockNumber < 11) {
		return;
	}
	else if (blockNumber == 11) {
		//clear old state by creating a ..1100011... mask and ANDing
		u32 mask = 0xFFFFFFF8; //...1111 1000
		u32 oneFill = 0x3FFFFFFF >> (30 - (27));
		mask = (mask << (27)) | oneFill;
		tempState = tempState & mask;

		//Set the new state
		newErosion = (u32)erosion & 0x7; // this will pad the left with 0s
		newErosion = newErosion << (27);
		tempState = tempState | newErosion;
		bunkerState[bunkerNumber] = tempState;
	}
}

/**
 * Masks the u32 bunker state variable to return the
 * erosion state of the bunker block.
 *
 * @param blockNumber the block within the bunker (0-9)
 * @param bunkerNumber the number of the bunker (0-3)
 * @return the erosion state of the block specified 4 = dead, 0 = new
 */
u8 getBlockState(u8 bunkerNumber, u8 blockNumber) {
	u32 tempState = bunkerState[bunkerNumber];
	u8 erosionState;
	if (blockNumber < 9) {
		//Shift and mask only the 3 bits representing the state of the block
		erosionState = (u8)(tempState >> (blockNumber*3));
		erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	}
	else if (blockNumber < 11) {
		erosionState = 4;	// return a black block
	}
	else if (blockNumber == 11) {
		erosionState = (u8)(tempState >> (27));
		erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	}
	return erosionState;
}

/**
 * Returns the entire u32 containing all block's state data.
 *
 * @param bunkerNumber the number of the bunker (0-3)
 * @return Entire erosion state for the given bunker.
 */
u32 getBunkerState(u8 bunkerNumber) {
	return bunkerState[bunkerNumber];
}

/**
 * Gets the (x,y) coordinates of top-left corner of the bunker.
 * @param bunkerNumber The Bunker Number to check (0-3).
 * @return The (x,y) coordinates of the top-left corner of the bunker.
 */
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
