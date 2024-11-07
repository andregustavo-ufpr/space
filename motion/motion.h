#ifndef __motion__
#define __motion__
#endif

#include <stdio.h>

typedef struct position {
    int x;
    int y;
    int vel_x;
    int vel_y;
} position;

typedef struct collisionBox{
    position *coord;
    int height;
    int width;
} collisionBox;

// Set the position of each axis
void setXPos(int x, position *coord);
void setYPos(int y, position *coord);

// Update the position according with the velocities
void updatePos(position *coord);

int checkCollision(collisionBox *box1, collisionBox *box2);