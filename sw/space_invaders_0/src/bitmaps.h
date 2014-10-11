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
#define ALIEN_BULLET_SPEED 10   // # of pixel the alien bullets moves per update
#define ALIEN_BULLET_WIDTH 6
#define ALIEN_BULLET_HEIGHT 10
#define TANK_BULLET_WIDTH 2
#define TANK_BULLET_HEIGHT 10

u32 getTankPixel(u32 row, u32 col);
u32 getBunkerPixel(u32 row, u8 bunkerNumber, u8 blockNumber);
const u32* getAlienArray(u32 alienNumber);
const u32* getAlienBulletArray(u8 bulletNum);

const u32* getScoreText1();
const u32* getScoreText2();
const u32* getLivesText1();
const u32* getLivesText2();
const u32* getDigitArray(u8 digit);
const u32* getSpaceShipArray();

#endif
