#ifndef __motion__
#define __motion__

#include <stdio.h>
#include <stdlib.h>

#define X_AXIS 1
#define Y_AXIS 2

#define X_SCREEN 1600
#define Y_SCREEN 900

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

void addVelocity(int vel, position * coord, int axis);

// Update the position according with the velocities
void updatePos(position *coord);

int checkCollision(collisionBox *box1, collisionBox *box2);

position *createPosition(int x, int y);

void destroyPosition(position *coords);

collisionBox *createBox(int x, int y, int height, int width);

void destroyBox(collisionBox *box);

#endif