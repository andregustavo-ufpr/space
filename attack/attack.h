#ifndef __attack__
#define __attack__
#endif

#include "motion.h"
#include <allegro5/bitmap.h>

typedef struct attack {
    collisionBox *box;                                                          // Position and speed information
    int cooldown;                                                               // Time beetween one attack and the next
    int damage;                                                                 // How many HP this removes
    int expire_time;                                                            // Time in ticks that it stays active
    ALLEGRO_BITMAP *sprite;                                                     // Bitmap to represent attack
    struct attack *next;
} attack;

attack *createAttack(collisionBox *box, int cooldown, int damage, int expire_time);

void updateAttack(attack *atk, int clk);

void setAttackSpeed(attack *atk, int vel_x, int vel_y);

void deleteAttack(attack *atk);

