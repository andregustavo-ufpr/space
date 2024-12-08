#ifndef __attack__
#define __attack__

#include "motion.h"
#include <allegro5/bitmap.h>

typedef enum attacker {
    PLAYER,
    ALIEN
} attacker;


typedef struct attack {
    collisionBox *box;                                                          // Position and speed information
    int damage;                                                                 // How many HP this removes
    int expire_time;                                                            // Time in ticks that it stays active
    ALLEGRO_BITMAP *sprite;                                                     // Bitmap to represent attack
    bool used;
    attacker owner;
} attack;

attack *createAttack(collisionBox *box, int damage, int expire_time, attacker owner);

void updateAttack(attack *atk, int clk);

void setAttackSpeed(attack *atk, int vel_x, int vel_y);

void deleteAttack(attack *atk);

#endif