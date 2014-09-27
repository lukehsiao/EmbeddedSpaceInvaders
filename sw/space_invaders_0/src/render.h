/**
 * This interface will provide a structured way to handle rendering
 *
 * @author Luke Hsiao
 * @author Jeff Ravert
 * @date 23 Sept 2014
 */

#ifndef RENDER_H_
#define RENDER_H_
#include "globals.h"		//to give access to the global variables

/**
 * Draws all the items based on their current global variable values
 */
void render();
void unrender();
void blankScreen();
void parseKey(u8 keyPressed, u32 timerSeed);

#endif /* RENDER_H_ */
