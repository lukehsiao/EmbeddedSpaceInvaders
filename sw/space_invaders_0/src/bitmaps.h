/**
 * These are the prototypes and #defines for bitmaps.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */


#ifndef bitmaps_h
#define bitmaps_h
#include "globals.h"

// Packs each horizontal line of the figures const unsigned int a single 32 bit word.
#define packWord32(b31,b30,b29,b28,b27,b26,b25,b24,b23,b22,b21,b20,b19,b18,b17,b16,b15,b14,b13,b12,b11,b10,b9,b8,b7,b6,b5,b4,b3,b2,b1,b0) \
((b31 << 31) | (b30 << 30) | (b29 << 29) | (b28 << 28) | (b27 << 27) | (b26 << 26) | (b25 << 25) | (b24 << 24) |  \
 (b23 << 23) | (b22 << 22) | (b21 << 21) | (b20 << 20) | (b19 << 19) | (b18 << 18) | (b17 << 17) | (b16 << 16) |  \
 (b15 << 15) | (b14 << 14) | (b13 << 13) | (b12 << 12) | (b11 << 11) | (b10 << 10) | (b9  << 9 ) | (b8  << 8 ) |  \
 (b7  << 7 ) | (b6  << 6 ) | (b5  << 5 ) | (b4  << 4 ) | (b3  << 3 ) | (b2  << 2 ) | (b1  << 1 ) | (b0  << 0 ) )
#define ALIEN_HEIGHT 16
#define BUNKER_SQUARE_HEIGHT 12
#define ALIEN_BULLET_WIDTH 6
#define ALIEN_BULLET_HEIGHT 10
#define TANK_BULLET_WIDTH 2
#define TANK_BULLET_HEIGHT 10
#define GAME_OVER_HEIGHT 32

/**
 * This function returns the color of the pixel to draw assuming we're
 * already looking inside the small tank array.
 *
 * @param row The row number of the pixel within the tank
 * @param col The column number of the pixel within the tank
 * @return the color value to draw at that pixel
 */
u32 getTankPixel(u32 row, u32 col);

/**
 * This function returns a u32 representing 1 row of of one block
 * within the bunker. 
 *
 * @param row The row number to draw
 * @param bunkerNumber	number of the bunker (0-3)
 * @param blockNumber	number of the block(0-11)
 * @return The pixels present in the given row
 */
u32 getBunkerPixel(u32 row, u8 bunkerNumber, u8 blockNumber);

/**
 * This function calculates which alien array to return based on alien
 * number.
 *
 * @param alienNumber the number of the alien to display
 * @return a pointer to the 32x16 bit array of bitmap data.
 */
const u32* getAlienArray(u32 alienNumber);

/**
 * This function calculates which alien Bullet array to return based on Bullet
 * number.
 *
 * @param bulletNumber the number of the bullet to display
 * @return a pointer to the 32x10 bit array of bitmap data.
 */
const u32* getAlienBulletArray(u8 bulletNum);

/**
 * returns the array to print for the first half of the word "score".
 */
const u32* getScoreText1();

/**
 * returns the array to print for the second half of the word "score".
 */
const u32* getScoreText2();

/**
 * returns the array to print for the first half of the word "lives".
 */
const u32* getLivesText1();

/**
 * returns the array to print for the second half of the word "lives".
 */
const u32* getLivesText2();

/**
 * Returns the bitmap for the specified digit
 */
const u32* getDigitArray(u8 digit);

/**
 * Returns the UFO Array
 */
const u32* getSpaceShipArray();

/**
 * Returns the tank's normal bitmap
 */
const u32* getTankArray();

/**
 * Returns the tank's death bitmap 1
 */
const u32* getDeathTankArray1();

/**
 * Returns the tank's death bitmap 1
 */
const u32* getDeathTankArray2();

/**
 * Returns the alien death bitmap
 */
const u32* getDeadAlienArray();

/**
 * Returns a pointer to a letter of the word "GAME OVER"
 *
 * @param letterNum The number of the letter to return (e.g. 0=G, 1 = A)
 */
const u32* getGameOverArray(u8 letterNum);

#endif
