#include "entity.h"


entity *createEntity(collisionBox *box, int health, short int attack_type){
    entity *ent;

    ent = malloc(sizeof(entity));

    if(ent == NULL){
        printf("Failed to create entity");
        return NULL;
    }

    ent->box = box;
    ent->health = health;
    ent->attack_type = attack_type;

    return ent;
}

void deleteEntity(entity *ent){
    if(ent == NULL){
        return;
    }

    free(ent);
}