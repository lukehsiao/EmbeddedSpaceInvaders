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

void updateAlienLocation() {
	static u8 direction = 1;	//1 = right, 0 = left
	// Update Location each call
	point_t tempAlien = getAlienBlockPosition();

	if(direction == 1){
		tempAlien.x = tempAlien.x + X_SHIFT;
	}
	else {
		tempAlien.x = tempAlien.x - X_SHIFT;
		if(tempAlien.x > 640) {
			tempAlien.x = 0;
		}
	}
	// Alien Block hit Right side
	if(tempAlien.x >= (640-32*11) && direction == 1) {
		tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
		direction = 0;
	} // Alien Block hit left side
	else if(tempAlien.x <= 5 && direction == 0) {
			tempAlien.y = tempAlien.y + ALIEN_HEIGHT;
			direction = 1;
	}


	tempAlien.y = tempAlien.y + 0;
	setAlienBlockPosition(tempAlien);
}

void unrenderAliens() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	point_t position;
	position = getAlienBlockPosition();
	for (row = 0; (16+10)*5; row++) {
		for (col = 0; col < (11*32); col++) {
			framePointer0[(position.y + row)*640 + (position.x+col)] = 0x0;
		}
	}
}

/**
 *  Draws all the aliens.  Note that it will toggle the alien guise each time it is called
 */
void renderAliens() {
	//unrenderAliens();
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
	int col;
	int row;
	int alienNumber;
	point_t position = getAlienBlockPosition();
	u8 tempGuise = toggleAlienGuise();
	for(alienNumber = 0; alienNumber < 11; alienNumber++) {
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y + row)*640 + (position.x+col+(alienNumber*32))] = getAlienPixel(row, col, 0, tempGuise, alienNumber);
			}
		}
	}
	for(alienNumber = 11; alienNumber < 22; alienNumber++) {
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y + row + ALIEN_HEIGHT + 10)*640 + (position.x+col+((alienNumber%11)*32))] = getAlienPixel(row, col, 1, tempGuise, alienNumber);
			}
		}
	}
	for(alienNumber = 22; alienNumber < 33; alienNumber++) {
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y + row + 2*(ALIEN_HEIGHT + 10))*640 + (position.x+col+((alienNumber%11)*32))] = getAlienPixel(row, col, 1, tempGuise, alienNumber);
			}
		}
	}
	for(alienNumber = 33; alienNumber < 44; alienNumber++) {
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y + row + 3*(ALIEN_HEIGHT + 10))*640 + (position.x+col+((alienNumber%11)*32))] = getAlienPixel(row, col, 2, tempGuise, alienNumber);
			}
		}
	}
	for(alienNumber = 44; alienNumber < 55; alienNumber++) {
		for (row = 0; row < ALIEN_HEIGHT; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(position.y + row + 4*(ALIEN_HEIGHT + 10))*640 + (position.x+col+((alienNumber%11)*32))] = getAlienPixel(row, col, 2, tempGuise, alienNumber);
			}
		}
	}
	updateAlienLocation();
}

void blankScreen() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
		int row=0, col=0;
		for( row=0; row<480; row++) {
			for(col=0; col<640; col++) {
				if(row < 240) {
					if(col<320) {
						// upper left corner.
						framePointer0[row*640 + col] = 0x00000000;  // frame 0 is red here.
					} else {
						// upper right corner.
						framePointer0[row*640 + col] = 0x00000000;  // frame 0 is blue here.
					}
				} else {
					if(col<320) {
						// lower left corner.
						framePointer0[row*640 + col] = 0x00000000;  // frame 0 is green here.
					} else {
						// lower right corner.
						framePointer0[row*640 + col] = 0x00000000;  // frame 0 is white here.
					}
				}
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

