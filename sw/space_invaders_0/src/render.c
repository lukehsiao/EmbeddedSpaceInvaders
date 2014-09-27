/**
 * This implements render.h
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#define FRAME_BUFFER_ADDR 0xC0000000
#include "render.h"


/**
 * Draws the tank in it's current position
 */
void renderTank() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;

	int col;
	int row;
	for (row = 0; row < 16; row++) {
		for (col = 0; col < 32; col++) {
			framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = getTankPixel(row, col); //Green
		}
	}
}

void renderBunker(u8 bunkerNumber){
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	int blockNum;
	for (blockNum = 0; blockNum < 4; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + row)*640 + (getBunkerPosition(bunkerNumber).x+(blockNum*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
	for (blockNum = 4; blockNum < 8; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + 12 + row)*640 + (getBunkerPosition(bunkerNumber).x+((blockNum-4)*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
	for (blockNum = 8; blockNum < 12; blockNum++) {
		for (row = 0; row < 12; row++) {
			for (col = 0; col < 12; col++) {
				framePointer0[(getBunkerPosition(bunkerNumber).y + 24 + row)*640 + (getBunkerPosition(bunkerNumber).x+((blockNum-8)*12)+col)] = getBunkerPixel(row, col, bunkerNumber, blockNum); //Green
			}
		}
	}
}


/**
 * Deletes only the necessary pixels when the aliens are moving right or left.
 * Then, blanks the entire block when the aliens drop a row.
 */
void unrenderAliens() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	point_t position;
	position = getAlienBlockPosition();
	static u8 direction;
	direction = getAlienDirection();

	//If moving right..
	if(direction == 1){
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < X_SHIFT; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
			}
		}
	}
	// if X_SHIFT > 6, we'll need to add logic hear to clear the right side as well.

	// If we hit the right edge
	if(position.x >= (640-32*11) && direction == 1) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
			}
		}
	} // Alien Block hit left side
	else if(position.x <= 5 && direction == 0) {
		for (row = 0; row < (ALIEN_HEIGHT + 10)*5; row++) {
			for (col = 0; col < 32*11; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col)] = BLACK;
			}
		}
	}
}

/**
 * Contains the algorithm to move the aliens left and right, as well as drop
 * rows when they hit the edge of the screen.
 */
void updateAlienLocation() {
	unrenderAliens();
	u8 direction = getAlienDirection();
	// Update Location each call
	point_t tempAlien = getAlienBlockPosition();

	// Alien Block hit Right side
	if(tempAlien.x >= (640-32*11) && direction == 1) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		setAlienDirection(0);
	} // Alien Block hit left side
	else if(tempAlien.x <= 5 && direction == 0) {
			tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
			setAlienDirection(1);
	}
	else {
		if(direction == 1){
			tempAlien.x = tempAlien.x + X_SHIFT;
		}
		else {
			tempAlien.x = tempAlien.x - X_SHIFT;
			if(tempAlien.x > 640) {
				tempAlien.x = 0;
			}
		}
	}
	setAlienBlockPosition(tempAlien);
}

/**
 *  Draws all the aliens.  Note that it will toggle the alien guise each time it is called
 */
void renderAliens() {
	updateAlienLocation();
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	int alienNumber;
	point_t position = getAlienBlockPosition();
	toggleAlienGuise();
	const u32* arrayToRender;
	for (alienNumber = 0; alienNumber < 55; alienNumber++) {
		//algorithm to adjust x and y for drawing
		if (alienNumber != 0) {
			position.x = position.x + 32;
			if (alienNumber % 11 == 0) {	//if end of row is reached, increment
				position.y = position.y + (ALIEN_HEIGHT + 10);
				position.x = getAlienBlockPosition().x;
			}
		}

		//Rendering each alien
		arrayToRender = getAlienArray(alienNumber);
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				u8 pixelPresent = (arrayToRender[row] >> (31-col)) & 0x1;
				framePointer0[(position.y + row)*640 + (position.x + col)] = pixelPresent? WHITE : BLACK;
			}
		}
	}
}

/**
 * Writes Black to the entire screen
 */
void blankScreen() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
		int row=0, col=0;
		for( row=0; row<480; row++) {
			for(col=0; col<640; col++) {
				framePointer0[row*640 + col] = BLACK;  // frame 0 is red here.
			}
		}
}

void render() {
	//blankScreen();
	renderTank();
	renderBunker(0);
	renderBunker(1);
	renderBunker(2);
	renderBunker(3);
	renderAliens();
}

void unrender() {
	// Unrender tanks
	unrenderAliens();
	// unrender aliens

	// unrender bullets
}

