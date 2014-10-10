/**
 * Contains all the prototypes for rendering the aliens.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef ALIENS_H_
#define ALIENS_H_

void unrenderAliens();
void updateAlienLocation();
void adjustPadding();
void killAlien(u32 alienNumber);
void renderAliens(u8 animate);
void fireAlienBullet(u32 randomCol);
void renderAlienBullet(u8 animate);

#endif /* ALIENS_H_ */
