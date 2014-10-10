/*
 * Implements bunkers.h and contains the bitmaps for the bunkers.
 *
 *  @author Luke Hsiao
 *  @author Jeff Revert
 *  @date 10 Oct 2014
 */
#include "globals.h"
#include "bunkers.h"

extern u32* framePointer0;

/**
 * Draws the bunker
 * @param bunkerNumber The bunker to draw (0-3)
 */
void renderBunker(u8 bunkerNumber){
	u32 col;
	u32 row;
	u32 blockNum;
	point_t bunkerPosition;
	u32 rowsPixels;
	u8 pixelPresent;
	bunkerPosition = getBunkerPosition(bunkerNumber);
	for (blockNum = 0; blockNum < 4; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + row)*640 + (bunkerPosition.x+(blockNum*12))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = 4; blockNum < 8; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 12 + row)*640 + (bunkerPosition.x+((blockNum-4)*12))+col] = BLACK;
					}
				}
			}
		}
	}
	for (blockNum = 8; blockNum < 12; blockNum++) {
		for (row = 0; row < 12; row++) {
			rowsPixels = getBunkerPixel(row, bunkerNumber, blockNum);
			for (col = 0; col < 12; col++) {
				pixelPresent = (rowsPixels >> (31 - col)) & 0x1;
				if (pixelPresent) {
					if (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = GREEN;
					}
				}
				else {
					if (framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] != WHITE) {
						framePointer0[(bunkerPosition.y + 24 + row)*640 + (bunkerPosition.x+((blockNum-8)*12))+col] = BLACK;
					}
				}
			}
		}
	}
}
