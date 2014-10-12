/**
 * Contains all the prototypes for rendering the spaceship.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef SPACESHIP_H_
#define SPACESHIP_H_

#define SPACESHIP_START_RATE 1//1000000
#define SPACESHIP_SPEED 4
#define SPACESHIP_SCORE_CYCLES 1
#define SPACESHIP_SCORE_COUNT 1
#define SPACESHIP_SCORE_STEADY 8


void renderSpaceShip();
void unrenderSpaceShip();
void updateSpaceShipLocation();
void startSpaceShip();
void renderPoints(u32 points, point_t position);
void unrenderPoints(point_t position);
u8 hitSpaceShip(point_t bulletPosition);
void setActivated(u8 val);
u8 getActivated();
void setSpaceshipScore(u32 val);
u32 getSpaceshipScore();

#endif /* SPACESHIP_H_ */
