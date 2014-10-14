/**
 * Contains all the prototypes for rendering the aliens.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef ALIENS_H_
#define ALIENS_H_

#define X_SHIFT 6           // # of pixels the Alien Block shifts horizontally.
#define ALIEN_BULLET_FIRE_RATE 20
#define ALIEN_BULLET_SPEED 6   // # of pixel the alien bullets moves per update
#define BOTTOM_BORDER 418
#define ALIEN_EXPLOSION_SHOW_TIME 2   // # of pixel the alien bullets moves per update
#define ALIEN_STATE_MACHINE_RATE_MAX 60 // this is the slowest that the Aliens SM will be called
#define ALIEN_STARTING_Y_POSITION 70

void unrenderAliens();
void updateAlienLocation();
void adjustPadding();
void killAlien(u8 alienNumber);
void unrenderDeadAlien(point_t position);
void renderAliens(u8 animate);
void fireAlienBullet(u32 randomCol);
void renderAlienBullet(u8 animate);
u8 calculateAlienBulletHit(alienBullet bullet);
u8 hitAlien(point_t position);

#endif /* ALIENS_H_ */
