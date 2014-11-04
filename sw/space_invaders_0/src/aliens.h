/**
 * Contains all the prototypes for rendering the aliens.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef ALIENS_H_
#define ALIENS_H_

#define X_SHIFT 6           			// # of pixels the Alien Block shifts horizontally.
#define ALIEN_BULLET_FIRE_RATE 40		// Increase to have aliens fire less often
#define ALIEN_BULLET_SPEED 6   			// # of pixel the alien bullets moves per update
#define BOTTOM_BORDER 418      			// The lowest Y position the aliens can go before game over happens
#define ALIEN_EXPLOSION_SHOW_TIME 2   	// # of pixel the alien bullets moves per update
#define ALIEN_STATE_MACHINE_RATE_MAX 60 // this is the slowest that the Aliens SM will be called
#define ALIEN_STARTING_Y_POSITION 70

#define ALIEN_ARRAY_WIDTH 32
#define ALIEN_ACTUAL_WIDTH 26

#define ALIEN_GUISE_0 0
#define ALIEN_GUISE_1 1
#define ALIEN_ROWS 5
#define ALL_ALIENS_ALIVE 0xFFFF

#define MAX_ALIEN_NUMBER_IN_ROW 11
#define MAX_ALIEN_NUMBER_IN_COLUMN 5

#define MAX_ROW_ONE_ALIEN_NUMBER 11
#define MAX_ROW_TWO_ALIEN_NUMBER 22
#define MAX_ROW_THREE_ALIEN_NUMBER 33
#define MAX_ROW_FOUR_ALIEN_NUMBER 44
#define MAX_ROW_FIVE_ALIEN_NUMBER 55

#define MAX_ALIEN_NUMBER 55

#define ROW_ONE 0
#define ROW_TWO 1
#define ROW_THREE 2
#define ROW_FOUR 3
#define ROW_FIVE 4

#define BULLET_ZERO 0
#define BULLET_ONE 1
#define BULLET_TWO 2
#define BULLET_THREE 3

#define BULLET_GUISE_ZERO 0
#define BULLET_GUISE_ONE 1

#define ALIEN_RIGHT_SIDE_BLANK 10


#define MOST_SIGIFICANT_BIT 15


#define STARTING_LEFT_MOST_COLUMN 0
#define STARTING_RIGHT_MOST_COLUMN 10
#define STARTING_LEFT_PAD 0
#define STARTING_RIGHT_PAD 640
#define STARTING_ALIEN_BLOCK_X 160
#define STARTING_ALIEN_BLOCK_Y 70
#define STARTING_ALIEN_SOUND_NUMBER 1
#define STARTING_LOWEST_ALIEN_ALIVE_ROW 4
#define MAX_ALIEN_SOUND_NUMBER 4
#define ALIEN_DIRECTION_RIGHT 1
#define ALIEN_DIRECTION_LEFT 0
#define MAX_NUMBER_ALIENS_ALIVE 55


#define MAX_BULLET_NUM 4
#define NUM_BULLET_GUISE 2
#define OFF_THE_SCREEN 8888

#define TOP_ROW_SCORE 40
#define MIDDLE_ROW_SCORE 20
#define BOTTOM_ROW_SCORE 10
#define NUMBER_MOD 10
#define SCORE_WIDTH 10
#define HUNDRED 100
#define THOUSAND 1000
#define TEN_THOUSAND 10000

#define FIND_MIDDLE_BULLET 2
#define BULLET_NEAR_BUNKERS 379

#define WITHOUT_MOVING 0
#define WITH_MOVING 1


/**
 * Deletes only the necessary pixels when the aliens are moving right or left.
 * Then, blanks the entire block when the aliens drop a row.
 */
void unrenderAliens();

/**
 * Contains the algorithm to move the aliens left and right, as well as drop
 * rows when they hit the edge of the screen.
 */
void updateAlienLocation();

/**
 * If the killed alien is on an edge column, traverses up to see if the
 * column is empty and adjusts the global variables.
 */
void adjustPadding();

/**
 * Draws the death guise for the alien at the specified number
 */
void killAlien(u8 alienNumber);

/**
 * Erases the Death Guise at the position specified.
 */
void unrenderDeadAlien(point_t position);

/**
 *  Draws all the aliens.  Note that it will toggle the alien guise each time it is called
 *  @param animate 1 = toggle guise and move location, 0 = just refresh
 */
void renderAliens(u8 animate);

/**
 * Fires an alien bullet if available from a random bottom row alien
 */
void fireAlienBullet(u32 randomCol);

/**
 *  Draws all the alien bullets.
 *  @param animate 1=update position, 0=refresh only
 */
void renderAlienBullet(u8 animate);

/**
 * Calculates whether the bullet hit the tank or the bunkers.  This
 * will update global flags so that we can transition to death animations
 *
 * @param bullet The Alien Bullet to evaluate
 * @return 1=hit, 0=no hit
 */
u8 calculateAlienBulletHit(alienBullet bullet);

/**
 * Calculates whether an alienBullet hit a tank
 *
 * @param position The position to test
 * @return The number of the alien hit, 0xFF if none.
 */
u8 hitAlien(point_t position);

#endif /* ALIENS_H_ */
