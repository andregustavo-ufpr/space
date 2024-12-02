#ifndef __attack__

#define MAX_ATTACKS 100
#define __attack__

#include "motion.h"
#include <allegro5/bitmap.h>



typedef struct attack {
    collisionBox *box;                                                          // Position and speed information
    int damage;                                                                 // How many HP this removes
    int expire_time;                                                            // Time in ticks that it stays active
    ALLEGRO_BITMAP *sprite;                                                     // Bitmap to represent attack
} attack;

typedef struct attackNode{
    attack *info;
    struct attackNode *next;
} attackNode;

typedef struct attackQueue{
    attackNode *front;
    attackNode *rear;
} attackQueue;

attackNode *createNode(attack* attack_info);

attackQueue *appendNode(attackQueue *queue, attack *data);

attack* deleteNode(attackQueue *queue);

attack *createAttack(collisionBox *box, int damage, int expire_time);

void updateAttack(attack *atk, int clk);

void setAttackSpeed(attack *atk, int vel_x, int vel_y);

void deleteAttack(attack *atk);

#endif