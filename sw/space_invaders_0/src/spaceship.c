/**
 * This implements spaceships.h
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "globals.h"
#include <stdlib.h>
#include "render.h"
#include "spaceInvadersSounds.h"

extern u32* framePointer0;

static u8 direction; // 1 = right, 0 = left

/**
 * Unrenders the SpaceShip at it's current location
 */
void unrenderSpaceShip() {
	u32 col;
	u32 row;
	point_t position;
	position = getSpaceshipPosition();
	if (direction == 1) {
		for (row = 0; row < 16; row++) {
			for (col = 0; col < SPACESHIP_SPEED; col++) {
				if (framePointer0[(position.y+row)*640 + (position.x+col)] != YELLOW) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	}
	else {
		for (row = 0; row < 16; row++) {
			for (col = 31; col > (31-SPACESHIP_SPEED); col--) {
				if (framePointer0[(position.y+row)*640 + (position.x+col)] != YELLOW) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = BLACK;
				}
			}
		}
	}

}

/**
 * Call this to put the spaceship in motion
 */
void startSpaceShip() {
	point_t position;
	if (getSpaceshipActivated() != 1) {
		unrenderSpaceShip();
		setSpaceshipActivated(1);
		direction = (direction ^ 0x1) & 0x1;		// choose opposite direction
		position.y = 35;
		if (direction == 1) {
			position.x = 0;
		}
		else {
			position.x = 640-32;
		}
		setSpaceshipPosition(position);
		renderSpaceShip();
	}
}

/**
 * Updates the SpaceShip's location
 */
void updateSpaceShipLocation() {
	unrenderSpaceShip();
	point_t position;
	position = getSpaceshipPosition();

	// If moving right
	if (direction == 1) {
		position.x += SPACESHIP_SPEED;
		setSpaceshipPosition(position);
		if (position.x > 640) {
			// Trigger Tank Death Sound
			setActive(SPACESHIP_MOVE_NUM, INACTIVE);
			setSpaceshipActivated(0);
			setCurrentSampleNum(SPACESHIP_MOVE_NUM, 0);
		}
	}
	else {
		position.x -= SPACESHIP_SPEED;
		setSpaceshipPosition(position);
		if ((position.x+32) > 640) { //if it's off the screen to the left
			setActive(SPACESHIP_MOVE_NUM, INACTIVE);
			setSpaceshipActivated(0);
			setCurrentSampleNum(SPACESHIP_MOVE_NUM, 0);
		}
	}
}

/**
 * Draws the Spaceship
 */
void renderSpaceShip() {
	u32 col;
	u32 row;
	point_t position;
	const u32* arrayToRender;
	if (getSpaceshipActivated()) {
		setActive(SPACESHIP_MOVE_NUM, ACTIVE);
		updateSpaceShipLocation();
		position = getSpaceshipPosition();
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
 *  Renders the number of points passed in at the location of the spaceship
 *
 * @param points The number of points to display
 * @param position The top-left corner of where to draw the points.
 */
void renderPoints(u32 points, point_t position) {
	u8 ones, tens, hundreds;
	point_t localPosition = position;
	//This code assumes that the score will never exceed 9999

	ones = points % 10;
	tens = (points / 10) % 10;
	hundreds = (points / 100) % 10;

	u32 row, col;
	const u32* arrayToRender;
	if (points < 10) {
		localPosition.x += 15; //to center it better
		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
	}
	else if (points < 100) {
		localPosition.x += 5;	//to center it better
		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
		localPosition.x += 11;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
	}
	else {
		arrayToRender = getDigitArray(hundreds);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
		localPosition.x += 11;

		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
		localPosition.x += 11;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					if ((localPosition.x+col) < 640) {
						framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = YELLOW;
					}
				}
				else {
					framePointer0[(localPosition.y+row)*640 + (localPosition.x+col)] = BLACK;
				}
			}
		}
	}
}

/**
 * Blanks the spot where the score is to help with flashing
 *
 * @param position The top-left position of where to start erasing.
 */
void unrenderPoints(point_t position) {
	u32 row, col;
	for (row = 0; row < 16+2; row++) {
		for (col = 0; col < 32+4; col++) {
			framePointer0[(position.y+row-2)*640 + (position.x+col-2)] = BLACK; //Green
		}
	}
}

/**
 * Calculates whether the spaceship was hit or not.
 *
 * @param bulletPosition The position to test.
 * @return 1 = hit, 0 = not hit
 */
u8 hitSpaceShip(point_t bulletPosition) {
	point_t position;
	if (!getSpaceshipActivated()) {
		return 0;
	}
	position = getSpaceshipPosition();
	//If it's outside the spaceship
	if ((bulletPosition.x+32) < (position.x+32) || bulletPosition.y < position.y) {
		return 0;
	}
	else if (bulletPosition.x > (position.x+32) || bulletPosition.y > (position.y+16)) {
		return 0;
	}
	else {
		// Otherwise, it must be in the spaceship.
		// Trigger Tank Death Sound
		setActive(SPACESHIP_DEATH_NUM, ACTIVE);
		setSpaceshipActivated(0);
		setSpaceshipDied(1);
		unrenderPoints(position);
		u32 tempScore = ((rand() % 7)+1) * 50;
		setSpaceshipScore(tempScore);
		renderPoints(tempScore, position);
		u32 score = getScore();
		score += tempScore;
		setScore(score);
		renderScore();
		return 1;
	}
}
