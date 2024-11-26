#include "attack.h"

attack *createAttack(collisionBox *box, int cooldown, int damage, int duration){
    attack *atk;

    atk = malloc(sizeof(attack));
    if(atk == NULL){
        printf("Failed to create attack");
        return NULL;
    }
    
    atk->box = box;
    atk->damage = damage;
    atk->expire_time = duration;

    return atk;
}

void updateAttack(attack *atk, int clk){
    if(clk > atk->expire_time){
        deleteAttack(atk);
    }

    updatePos(atk->box->coord);
}

void setAttackSpeed(attack *atk, int vel_x, int vel_y){
    addVelocity(vel_x, atk->box->coord, X_AXIS);
    addVelocity(vel_y, atk->box->coord, Y_AXIS);
}


void deleteAttack(attack *atk){

    if(atk == NULL){
        return;
    }

    free(atk);
}