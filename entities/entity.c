#include "entity.h"

entity *createEntity(collisionBox *box, int health){
    entity *ent;

    ent = malloc(sizeof(entity));

    if(ent == NULL){
        printf("Failed to create entity");
        return NULL;
    }

    ent->box = box;
    ent->health = health;

    return ent;
}

void deleteEntity(entity *ent){
    if(ent == NULL){
        return NULL;
    }

    free(ent);
}