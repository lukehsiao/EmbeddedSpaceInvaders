/**
 * This implements render.h
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "render.h"
#include "aliens.h"
#include "tank.h"
#include "bunkers.h"

extern u32* framePointer0;


/**
 * Writes Black to the entire screen
 */
void blankScreen() {
	u32 row=0, col=0;
	for( row=0; row<480; row++) {
		for(col=0; col<640; col++) {
			framePointer0[row*640 + col] = BLACK;  // frame 0 is red here.
		}
	}
}

/**
 * Draws the green line at the bottom of the screen
 */
void renderBottomLine() {
	u32 col;
	u32 row;
	row = 479;
	for(col=0; col<640; col++) {
		framePointer0[row*640 + col] = GREEN;  // frame 0 is red here.
	}
}

//////////////////////////////////////////////////////////////////
// Functions for Rendering Score/Lives
//////////////////////////////////////////////////////////////////

/**
 * Renders the word "SCORE" at the top left of the screen
 */
void renderScoreText() {
	u32 row, col;
	const u32* arrayToRender;
	arrayToRender = getScoreText1();
	point_t defaultPosition;
	defaultPosition.x = 5;
	defaultPosition.y = 11;
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(defaultPosition.y+row)*640 + (defaultPosition.x+col)] = WHITE;  // frame 0 is red here.
			}
		}
	}

	arrayToRender = getScoreText2();
	defaultPosition.x = 5+32;
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(defaultPosition.y+row)*640 + (defaultPosition.x+col)] = WHITE;  // frame 0 is red here.
			}
		}
	}
}

/**
 * Renders the numeric value of the player's current score.
 */
void renderScore() {
	u32 score = getScore();
	u8 ones, tens, hundreds, thousands;
	//This code assumes that the score will never exceed 9999

	ones = score % 10;
	tens = (score / 10) % 10;
	hundreds = (score / 100) % 10;
	thousands = (score / 1000) % 10;

	u32 row, col;
	const u32* arrayToRender;
	point_t position;
	position.x = 80;
	position.y = 11;
	if (score < 10) {
		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
	}
	else if (score < 100) {
		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
	}
	else if (score < 1000) {
		arrayToRender = getDigitArray(hundreds);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
	}
	else if (score < 10000) {
		arrayToRender = getDigitArray(thousands);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(hundreds);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(tens);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
		position.x += 12;

		arrayToRender = getDigitArray(ones);
		for(row = 0; row < ALIEN_HEIGHT; row++) {
			for(col = 0; col < 10; col++) {
				if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
					framePointer0[(position.y+row)*640 + (position.x+col)] = GREEN;  // frame 0 is red here.
				}
			}
		}
	}
}

/**
 * Renders the word "LIVES" at the top right of the screen
 */
void renderLivesText() {
	u32 row, col;
	const u32* arrayToRender;
	arrayToRender = getLivesText1();
	point_t defaultPosition;
	defaultPosition.x = 350;
	defaultPosition.y = 11;
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(defaultPosition.y+row)*640 + (defaultPosition.x+col)] = WHITE;  // frame 0 is red here.
			}
		}
	}

	arrayToRender = getLivesText2();
	defaultPosition.x += 32;
	for(row = 0; row < ALIEN_HEIGHT; row++) {
		for(col = 0; col < 32; col++) {
			if (((arrayToRender[row] >> (31-col)) & 0x1) == 1) {
				framePointer0[(defaultPosition.y+row)*640 + (defaultPosition.x+col)] = WHITE;  // frame 0 is red here.
			}
		}
	}
}

/**
 * Renders the number of extra lives the player has as tank icons
 */
void renderLives() {
	u8 lives = getLives();
	point_t position;
	position.x = 420;
	position.y = 5;

	u32 col;
	u32 row;
	for (; lives > 0; lives--) {
		// Same Algorithm as Render Tank
		for (row = 0; row < 16; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y+row)*640 + (position.x+col)] = getTankPixel(row, col); //Green
			}
		}
		position.x += 42;
	}
}



/**
 * Renders all of the objects on the screen
 */
void render() {
	//blankScreen();
	renderSpaceShip();
	renderScoreText();
	renderScore();
	renderLivesText();
	renderLives();
	renderTank();
	renderTankBullet(1);
	renderAlienBullet(1);
	renderAliens(1);
	renderBunker(0);
	renderBunker(1);
	renderBunker(2);
	renderBunker(3);
	renderBottomLine();
}

/**
 * Specifically for Lab 3.  Parses the keyboard inputs.
 */
void parseKey(u8 keyPressed, u32 timerSeed, u32 userInput) {
	point_t temp;
	u8 random;
	u8 blockNumber;
	u8 erosionState;
	switch (keyPressed) {
		case '4':
			unrenderTank();
			temp = getTankPositionGlobal();
			temp.x = temp.x - 4;
			if (temp.x > (640-32)) {
				temp.x = 0;
			}
			setTankPositionGlobal(temp.x);
			renderTank();
			break;
		case '6':
			unrenderTank();
			temp = getTankPositionGlobal();
			temp.x = temp.x + 4;
			if (temp.x > (640-32)) {
				temp.x = (640-32);
			}
			setTankPositionGlobal(temp.x);
			renderTank();
			break;
		case '8':
			render();
			break;
		case '2':
			setAlienStatus(userInput, 0); //kill the random alien
			killAlien(userInput);
			break;
		case '1':
			startSpaceShip();
			break;
		case '5':
			//fire bullet
			fireTankBullet();
			break;
		case '3':
			//fire random alien missile
			random = ((timerSeed * 13) / 3) % 11; //pseudo random number between 0-10
			fireAlienBullet(random);
			break;
		case '9':
			renderTankBullet(1);
			renderAlienBullet(1);
			break;
		case '7':
			for (blockNumber = 0; blockNumber < 12; blockNumber++) {
				erosionState = getBlockState(userInput, blockNumber);
				if (erosionState < 4) {
					setBlockState(userInput, blockNumber, erosionState+1);
					break;
				}
			}
			renderBunker(userInput);
			break;
		default:
			//do nothing
			break;
	}
}




