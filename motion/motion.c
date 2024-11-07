#include "motion.h"

void setXPos(int x, position *coord){
    coord->x = x;
}

void setYPos(int y, position *coord){
    coord->y = y;
}

void updatePos(position *coord){
    coord->x = coord->x + coord->vel_x;
    coord->y = coord->y + coord->vel_y;
}

int checkColision(collisionBox *box1, collisionBox *box2){
    int overlap = 0;

    int boundaryX = box2->coord->x + box2->width;
    int boundaryY = box2->coord->y + box2->height;


    while(overlap != 1){
        if(box1->coord->x > box2->coord->x && box1->coord->x < boundaryX){
            overlap = 1;
        }
        if(box1->coord->y < box2->coord->y && box1->coord->y > boundaryY ){
            overlap = 1;
        }
    }
    
}