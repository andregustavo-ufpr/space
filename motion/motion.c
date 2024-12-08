#include "motion.h"

position *createPosition(int x, int y){
    position *new_position;

    new_position = malloc(sizeof(position));
    if(new_position == NULL){
        printf("Failed to create position");
        return NULL;
    }

    setXPos(x, new_position);
    setYPos(y, new_position);
    new_position->vel_x = 0;
    new_position->vel_y = 0;

    return new_position;
}

void destroyPosition(position *coords){
    free(coords);
}

collisionBox *createBox(int x, int y, int height, int width){
    collisionBox *new_box;

    new_box = malloc(sizeof(collisionBox));
    if(new_box == NULL){
        printf("Failed to create collision box");
        return NULL;
    }

    new_box->coord = createPosition(x, y);
    new_box->height = height;
    new_box->width = width;

    return new_box;
};

void destroyBox(collisionBox *box){
    destroyPosition(box->coord);

    free(box);
}

void setXPos(int x, position *coord){
    coord->x = x;
}

void setYPos(int y, position *coord){
    coord->y = y;
}

void addVelocity(int vel, position *coord, int axis){
    if(axis == X_AXIS){
        coord->vel_x += vel;
    }
    else{
        coord->vel_y += vel;
    }
}

void updatePos(position *coord){
    int next_x_pos = coord->x + coord->vel_x;
    int next_y_pos = coord->y + coord->vel_y;

    if(next_x_pos < 0 || next_x_pos > X_SCREEN){ 
        next_x_pos = coord->x;
    }

    if(next_y_pos < 0 || next_y_pos > (Y_SCREEN )){
        next_y_pos = coord->y;
    }

    coord->x = next_x_pos;
    coord->y = next_y_pos;
}

bool checkCollision(collisionBox *box1, collisionBox *box2){
    
    if(box1->coord->x > (box2->coord->x + box2->width)) return false;
    if((box1->coord->x + box1->width) < box2->coord->x) return false;
    if(box1->coord->y < (box2->coord->y - box2->height)) return false;
    if((box1->coord->y - box1->height) > box2->coord->y) return false;

    return true;
}