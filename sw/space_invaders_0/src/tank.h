/**
 * Contains all the prototypes for rendering the tank.
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 10 Oct 2014
 */

#ifndef TANK_H_
#define TANK_H_

#define TANK_INIT_POSITION_X 70
#define TANK_INIT_POSITION_Y 440

#define TANK_MOVEMENT_SPEED 3    // # of pixel the tank bullet moves per update
#define TANK_BULLET_SPEED 15    // # of pixel the tank bullet moves per update
#define TANK_MAP_FLIP_COUNT 2	// how long one cycle is for the tank death bitmap flipping
#define TANK_MAP_FLIP_CYCLES 10  // # of times the 2 bitmaps will show


void renderTank();
void unrenderTank();
void unrenderTankBullet();
void updateTankBulletPosition();
void renderTankBullet(u8 animate);
void fireTankBullet();
void moveTank(u32 position);
void moveTankLeft();
void moveTankRight();
u8 hitTank(point_t position);
u8 calculateTankBulletHit();
void deathTank1();
void deathTank2();
void setTankLife(u8 val);
u8 getTankLife();

#endif /* TANK_H_ */
