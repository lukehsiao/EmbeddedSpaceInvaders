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
point_t alienExplosion; // used for passing point of explosion between killAlien and AlienDeath_SM state machine
u8 alienDeath; // flag that is raised when an alien dies then is lowered when the AlienDeath_SM transitions
u8 numberAliensAlive; // keeps track of how many aliens are alive to calculate the rate

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

// Spaceship Variables
point_t spaceshipPosition;
u32 currentScore;
u8 spaceshipActivated; // 1 when spaceship is on the screen. 0 when the spaceship dies or is off the screen
u8 spaceshipDied; 		// flag that is set when ship is first hit then lowered when score is shown

// Score & Lives Variables
u32 score;
u8 lives;	//starts w/ 3 and decrements each death
u8 gameOver; // 1 is gameOver. 0 game is running
//******************* End Variables of Space Invaders**********************

/////////////////////////////////////
// Initializations
/////////////////////////////////////
void initGlobals(){
	xil_printf("\rInitializing Space Invaders Globals\n\r================\n\r");
	// Initialize Tank
	setTankPositionGlobal(TANK_STARTING_POSITION);
	tankLife = ALIVE;

	// Initialize Bunkers
    initBunkers();

    // Initialize Aliens
    alienGuise = ALIEN_GUISE_1;
	int i;
	for(i = 0; i < ALIEN_ROWS; i++) {
		alienStatus[i] = 0xFFFF;
	}
	leftMostColumn = STARTING_LEFT_MOST_COLUMN;
	rightMostColumn = STARTING_RIGHT_MOST_COLUMN;
	leftPad = STARTING_LEFT_PAD;
	rightPad = STARTING_RIGHT_PAD;
	point_t temp;
	temp.x = STARTING_ALIEN_BLOCK_X;
	temp.y = STARTING_ALIEN_BLOCK_Y;
	setAlienBlockPosition(temp);
	direction = ALIEN_DIRECTION_RIGHT;
	setNumberAliensAlive(MAX_NUMBER_ALIENS_ALIVE);

	// Initialize Game Variables
	setLives(STARTING_LIVES);
	setScore(STARTING_SCORE);
	gameOver = GAME_NOT_OVER;

	// Initialize Bullets
	alienBullet bullet;
	u8 j;
	for(j= 0; j < MAX_BULLET_NUM; j++){
		bullet = getAlienBullet(j);
		//temp.x = STARTING_TANK_POSITION_X + 150*j;
		temp.y = OFF_THE_SCREEN;	//not activated if >480
		bullet.position = temp;
		bullet.type = j%2;
		setAlienBullet(bullet, j);
	}
	temp.y = OFF_THE_SCREEN;	// also not actiavted
	temp.x = STARTING_TANK_POSITION_X;
	setTankBulletPosition(temp);

	//Initialize Spaceship
	spaceshipActivated = DEAD;
	spaceshipPosition.x = STARTING_SPACESHIP_POSITION_X;
	spaceshipPosition.y = STARTING_SPACESHIP_POSITION_Y;
	currentScore = STARTING_SCORE;

}


/////////////////////////////////////
// Implement the Spaceship Globals
/////////////////////////////////////
void setSpaceshipPosition(point_t val) {
	spaceshipPosition = val;
}
point_t getSpaceshipPosition() {
	return spaceshipPosition;
}

u32 getSpaceshipScore() {
	return currentScore;
}

void setSpaceshipScore(u32 val) {
	currentScore = val;
}

u8 getSpaceshipActivated() {
	return spaceshipActivated;
}

void setSpaceshipActivated(u8 val) {
	spaceshipActivated = val;
}

u8 getSpaceshipDied() {
	return spaceshipDied;
}

void setSpaceshipDied(u8 newVal) {
	spaceshipDied = newVal;
}

/////////////////////////////////////
// Implement the Tank Globals
/////////////////////////////////////
void setTankPositionGlobal(unsigned short val) {
	tankPosition.x = val;
	tankPosition.y = STARTING_TANK_POSITION_Y;
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
	if (alienNumber < MAX_ROW_ONE_ALIEN_NUMBER) {
		// clearing the bit
		u32 bitTemp = alienStatus[ROW_ONE] & ~(0x1 << (MOST_SIGIFICANT_BIT - alienNumber));
		// Setting the bit
		alienStatus[0] = bitTemp | (status << (MOST_SIGIFICANT_BIT - alienNumber));
	}
	else if (alienNumber < MAX_ROW_TWO_ALIEN_NUMBER) {
		// clearing the bit
		u32 bitTemp = alienStatus[ROW_TWO] & ~(0x1 << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
		// Setting the bit
		alienStatus[1] = bitTemp | (status << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
	}
	else if (alienNumber < MAX_ROW_THREE_ALIEN_NUMBER) {
		// clearing the bit
		u32 bitTemp = alienStatus[ROW_THREE] & ~(0x1 << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
		// Setting the bit
		alienStatus[2] = bitTemp | (status << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
	}
	else if (alienNumber < MAX_ROW_FOUR_ALIEN_NUMBER) {
		// clearing the bit
		u32 bitTemp = alienStatus[ROW_FOUR] & ~(0x1 << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
		// Setting the bit
		alienStatus[3] = bitTemp | (status << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
	}
	else {
		// clearing the bit
		u32 bitTemp = alienStatus[ROW_FIVE] & ~(0x1 << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
		// Setting the bit
		alienStatus[4] = bitTemp | (status << (MOST_SIGIFICANT_BIT - (alienNumber % MAX_ALIEN_NUMBER_IN_ROW)));
	}
}

/**
 * Read the alien status for the given alien.
 * @param alienNumber Which alien to get the status of.
 * @return 0 if dead, 1 if alive.
 */
u16 getAlienStatus(u32 alienNumber) {
	u16 temp;
	if (alienNumber < MAX_ROW_ONE_ALIEN_NUMBER) {
		temp = (alienStatus[ROW_ONE] >> (MOST_SIGIFICANT_BIT-alienNumber)) & 0x1;
	}
	else if (alienNumber < MAX_ROW_TWO_ALIEN_NUMBER) {
		temp = (alienStatus[ROW_TWO] >> (MOST_SIGIFICANT_BIT-(alienNumber % MAX_ALIEN_NUMBER_IN_ROW))) & 0x1;
	}
	else if (alienNumber < MAX_ROW_THREE_ALIEN_NUMBER) {
		temp = (alienStatus[ROW_THREE] >> (MOST_SIGIFICANT_BIT-(alienNumber % MAX_ALIEN_NUMBER_IN_ROW))) & 0x1;
	}
	else if (alienNumber < MAX_ROW_FOUR_ALIEN_NUMBER) {
		temp = (alienStatus[ROW_FOUR] >> (MOST_SIGIFICANT_BIT-(alienNumber % MAX_ALIEN_NUMBER_IN_ROW))) & 0x1;
	}
	else {
		temp = (alienStatus[ROW_FIVE] >> (MOST_SIGIFICANT_BIT-(alienNumber % MAX_ALIEN_NUMBER_IN_ROW))) & 0x1;
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
	case BULLET_ZERO:
		alienBullet_0 = val;
		break;
	case BULLET_ONE:
		alienBullet_1 = val;
		break;
	case BULLET_TWO:
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
	leftPad = LEFT_SIDE + (BOARDER*leftMostColumn);

}

/**
 * set the value of the left pad.
 */
void setRightCol(u8 rightCol) {
	rightMostColumn = rightCol;

	//Adjust the border
	rightPad = RIGHT_SIDE + (BOARDER*(ALIEN_RIGHT_SIDE_BLANK-rightMostColumn));
}

void setAlienExplosionPosition(point_t val) {
	alienExplosion = val;
}

point_t getAlienExplosionPosition() {
	return alienExplosion;
}

void setAlienDeath(u8 val) {
	alienDeath = val;
}

u8 getAlienDeath() {
	return alienDeath;
}

void setNumberAliensAlive(u8 val) {
	numberAliensAlive = val;
}

u8 getNumberAliensAlive() {
	return numberAliensAlive;
}

/////////////////////////////////////
// Implement the Bunker Globals
/////////////////////////////////////

/**
 * Initializes the bunkers in the correct positions
 */
void initBunkers() {
	int i;
	for (i = 0; i < TOTAL_BUNKER_NUM; i++) {
		bunkerState[i] = BUNKERS_NOT_ERODED;
	}
	bunkerPosition_0.x = BUNKER_ZERO_POSITION_X;
	bunkerPosition_0.y = ALL_BUNKER_POSITION_Y;

	bunkerPosition_1.x = BUNKER_ONE_POSITION_X;
	bunkerPosition_1.y = ALL_BUNKER_POSITION_Y;

	bunkerPosition_2.x = BUNKER_TWO_POSITION_X;
	bunkerPosition_2.y = ALL_BUNKER_POSITION_Y;

	bunkerPosition_3.x = BUNKER_THREE_POSITION_X;
	bunkerPosition_3.y = ALL_BUNKER_POSITION_Y;
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
	tempState = BUNKER_MASK & tempState;
	u32 newErosion;
	
	if (blockNumber < NUMBER_TOP_THREE_ROW_BLOCKS) {
		//clear old state by creating a ..1100011... mask and ANDing
		u32 mask = BUNKER_BLOCK_MASK; //...1111 1000
		u32 oneFill = BUNKER_MASK >> (BUNKER_MOST_SIGNIFICANT_BIT - (blockNumber*TOP_THREE_ROW_BLOCKS));
		mask = (mask << (blockNumber*TOP_THREE_ROW_BLOCKS)) | oneFill;
		tempState = tempState & mask;

		//Set the new state
		newErosion = (u32)erosion; // this will pad the left with 0s
		newErosion = newErosion << (blockNumber*TOP_THREE_ROW_BLOCKS);
		tempState = tempState | newErosion;
		bunkerState[bunkerNumber] = tempState;
	}
	else if (blockNumber < NUMBER_BOTTOM_RIGHT_BLOCK) {
		return;
	}
	else if (blockNumber == NUMBER_BOTTOM_RIGHT_BLOCK) {
		//clear old state by creating a ..1100011... mask and ANDing
		u32 mask = BUNKER_BLOCK_MASK; //...1111 1000
		u32 oneFill = BUNKER_MASK >> (BUNKER_MOST_SIGNIFICANT_BIT - (BOTTOM_RIGHT_BLOCK));
		mask = (mask << (BOTTOM_RIGHT_BLOCK)) | oneFill;
		tempState = tempState & mask;

		//Set the new state
		newErosion = (u32)erosion & 0x7; // this will pad the left with 0s
		newErosion = newErosion << (BOTTOM_RIGHT_BLOCK);
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
	if (blockNumber < NUMBER_TOP_THREE_ROW_BLOCKS) {
		//Shift and mask only the 3 bits representing the state of the block
		erosionState = (u8)(tempState >> (blockNumber*3));
		erosionState = 0x07 & erosionState;  // mask out extraneous top bits
	}
	else if (blockNumber < NUMBER_BOTTOM_RIGHT_BLOCK) {
		erosionState = DEAD_BLOCK;	// return a black block
	}
	else if (blockNumber == NUMBER_BOTTOM_RIGHT_BLOCK) {
		erosionState = (u8)(tempState >> (BOTTOM_RIGHT_BLOCK));
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
		case BUNKER_ZERO:
			return bunkerPosition_0;
		case BUNKER_ONE:
			return bunkerPosition_1;
		case BUNKER_TWO:
			return bunkerPosition_2;
		case BUNKER_THREE:
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

u8 getGameOver() {
	return gameOver;
}

void setGameOver(u8 newVal) {
	gameOver = newVal;
}



