#ifndef __ENTITY__
#define __ENTITY__
#endif

#include "motion.h"
#include <allegro5/bitmap.h>

typedef struct entity {
    collisionBox *box;
    int health;
    ALLEGRO_BITMAP *sprite;
} entity;

entity *createEntity(collisionBox *box, int health);

void deleteEntity(entity *ent);