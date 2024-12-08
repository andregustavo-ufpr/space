#ifndef __ALIEN__
#define __ALIEN__

#include <stdbool.h>
#include "motion.h"

typedef enum alienType {
    ALIEN_TYPE_BUG = 0,
    ALIEN_TYPE_ARROW,
    COLLECTABLE,
    ALIEN_TYPE_BIG,
    ALIEN_TYPE_N
} alienType;

typedef struct alien
{
    collisionBox *box;
    alienType type;
    int shot_timer;
    int health;
    bool used;
} alien;

#define MAX_ALIENS 20

void initAliens(alien *alien_list);

#endif 
