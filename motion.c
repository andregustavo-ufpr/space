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

int checkColision(position *coord1, position *coord2){
    
}