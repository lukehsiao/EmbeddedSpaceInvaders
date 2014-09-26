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

void renderBunkers(u8 bunkerNumber){
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;

		int col;
		int row;
		for (row = 0; row < 16; row++) {
			for (col = 0; col < 32; col++) {
				framePointer0[(getTankPositionGlobal().y+row)*640 + (getTankPositionGlobal().x+col)] = getTankPixel(row, col); //Green
			}
		}
}

void eraseTank() {

}

void render() {
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
	renderTank();
}

