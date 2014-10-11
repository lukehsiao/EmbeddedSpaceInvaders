/**
 * Contains all the prototypes for rendering the tank.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef TANK_H_
#define TANK_H_

#define TANK_MOVEMENT_SPEED 8    // # of pixel the tank bullet moves per update
#define TANK_BULLET_SPEED 15    // # of pixel the tank bullet moves per update
#define TANK_MAP_FLIP_COUNT 10	// how long one cycle is for the tank death bitmap flipping
#define TANK_MAP_FLIP_CYCLES 4  // # of times the 2 bitmaps will show

void renderTank();
void unrenderTank();
void unrenderTankBullet();
void updateTankBulletPosition();
void renderTankBullet(u8 animate);
void fireTankBullet();
void moveTankLeft();
void moveTankRight();
u8 hitTank(point_t position);
void deathTank1();
void deathTank2();

#endif /* TANK_H_ */
