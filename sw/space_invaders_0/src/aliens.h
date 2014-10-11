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
#define ALIEN_BULLET_FIRE_RATE 50

void unrenderAliens();
void updateAlienLocation();
void adjustPadding();
void killAlien(u8 alienNumber);
void unrenderDeadAlien(point_t position);
void renderAliens(u8 animate);
void fireAlienBullet(u32 randomCol);
void renderAlienBullet(u8 animate);
u8 calculateAlienBulletHit(alienBullet bullet);

#endif /* ALIENS_H_ */
