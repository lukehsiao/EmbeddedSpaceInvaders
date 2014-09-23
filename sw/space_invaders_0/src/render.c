/**
 * This implements render.h
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#include "render.h"
#include "globals.h"		//to give access to the global variables

#define FRAME_BUFFER_ADDR 0xC0000000


void renderTank() {
	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;

	int width;
	int height;
	for (height = 0; height < 10; height++) {
		for (width = 0; width < 32; width++) {
			framePointer0[(getTankPositionGlobal().y+height)*640 + (getTankPositionGlobal().x+width)] = 0x0000FF00; //Green
		}
	}
}

void render() {
	renderTank();
//	unsigned int* framePointer0 = (unsigned int *) FRAME_BUFFER_ADDR;
//	int row=0, col=0;
//	for( row=0; row<480; row++) {
//		for(col=0; col<640; col++) {
//			if(row < 240) {
//				if(col<320) {
//					// upper left corner.
//					framePointer0[row*640 + col] = 0x00000000;  // frame 0 is red here.
//				} else {
//					// upper right corner.
//					framePointer0[row*640 + col] = 0x00000000;  // frame 0 is blue here.
//				}
//			} else {
//				if(col<320) {
//					// lower left corner.
//					framePointer0[row*640 + col] = 0x00000000;  // frame 0 is green here.
//				} else {
//					// lower right corner.
//					framePointer0[row*640 + col] = 0x00000000;  // frame 0 is white here.
//				}
//			}
//		}
//	}
}

