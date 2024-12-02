#ifndef __ENTITY__
#define __ENTITY__

#include "motion.h"
#include <allegro5/bitmap.h>

#define MAIN_PLAYER 1
#define ENEMY 2
#define COLLECTABLE 3

typedef struct entity {
    collisionBox *box;
    int health;
    ALLEGRO_BITMAP *sprite;
    int type;
} entity;

entity *createEntity(collisionBox *box, int health);

void deleteEntity(entity *ent);

#endif