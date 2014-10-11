/**
 * This implements spaceships.h
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "globals.h"
#include "spaceship.h"

extern u32* framePointer0;

static u8 direction; // 1 = right, 0 = left
point_t position;
u8 activated;		// 1 = draw, 0 = ignore

/**
 * Call this to put the spaceship in motion
 */
void startSpaceShip() {
	if (activated != 1) {
		unrenderSpaceShip();
		activated = 1;
		direction = (direction ^ 0x1) & 0x1;		// choose opposite direction
		position.y = 35;
		if (direction == 1) {
			position.x = 0;
		}
		else {
			position.x = 640-32;
		}
		renderSpaceShip();
	}
}

/**
 * Draws the Spaceship
 */
void renderSpaceShip() {
	u32 col;
	u32 row;
	const u32* arrayToRender;
	if (activated) {
		updateSpaceShipLocation();
		arrayToRender = getSpaceShipArray();
		for (row = 0; row < 16; row++) {
			for (col = 0; col < 32; col++) {
				if ((arrayToRender[row] >> (31-col) & 0x1) == 1) {
					//Slide off the screen
					if ((position.x+col) < 640) {
						framePointer0[(position.y+row)*640 + (position.x+col)] = RED;
					}
				}
				else {
					framePointer0[(position.y+row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	}
}


/**
 * Unrenders the SpaceShip at it's current location
 */
void unrenderSpaceShip() {
	u32 col;
	u32 row;

	if (direction == 1) {
		for (row = 0; row < 16; row++) {
			for (col = 0; col < SPACESHIP_SPEED; col++) {
				framePointer0[(position.y+row)*640 + (position.x+col)] = BLACK; //Green
			}
		}
	}
	else {
		for (row = 0; row < 16; row++) {
			for (col = 31; col > (31-SPACESHIP_SPEED); col--) {
				framePointer0[(position.y+row)*640 + (position.x+col)] = BLACK; //Green
			}
		}
	}

}

/**
 * Updates the SpaceShip's location
 */
void updateSpaceShipLocation() {
	unrenderSpaceShip();
	// If moving right
	if (direction == 1) {
		position.x += SPACESHIP_SPEED;
		if (position.x > 640) {
			activated = 0;
		}
	}
	else {
		position.x -= SPACESHIP_SPEED;
		if ((position.x+32) > 640) { //if it's off the screen to the left
			activated = 0;
		}
	}
}

/**
 *  Renders the number of points passed in at the location of the spaceship
 */
void renderPoints(u32 points) {
	u8 ones, tens, hundreds;
	//This code assumes that the score will never exceed 9999

	ones = points % 10;
	tens = (points / 10) % 10;
	hundreds = (points / 100) % 10;

	u32 row, col;
	const u32* arrayToRender;
	point_t position;
	if (points < 10) {
		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
	}
	else if (points < 100) {
		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
	}
	else {
		arrayToRender = getDigitArray(hundreds);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = WHITE;  // frame 0 is red here.
				}
			}
		}
	}
}
