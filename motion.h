#ifndef __motion__
#define __motion__
#endif

#include <stdio.h>

typedef struct position {
    int x;
    int y;
    int vel_x;
    int vel_y;
    int height;
    int width;
} position;

// Set the position of each axis
void setXPos(int x, position *coord);
void setYPos(int y, position *coord);

// Update the position according with the velocities
void updatePos(position *coord);

int checkColision(position *coord1, position *coord2);