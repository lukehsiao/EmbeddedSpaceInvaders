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

void eraseTank() {

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
	blankScreen();
	renderTank();
	renderBunker(0);
	renderBunker(1);
	renderBunker(2);
	renderBunker(3);
}

