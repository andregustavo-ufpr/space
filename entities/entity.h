#ifndef __ENTITY__
#define __ENTITY__

#include "motion.h"
#include <allegro5/bitmap.h>

typedef struct entity {
    collisionBox *box;
    int health;
    ALLEGRO_BITMAP *sprite;
    short int attack_type;
    short int attack_timer;
    bool used;
} entity;

entity *createEntity(collisionBox *box, int health, short int attack_type);

void deleteEntity(entity *ent);

#endif