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
