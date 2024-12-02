#include "attack.h"

attackNode *createNode(attack* attack_info){
    attackNode* newNode = (attackNode* )malloc(sizeof(attackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->info = attack_info;
    newNode->next = NULL;
    return newNode;
}

attackQueue* createQueue() {
    attackQueue* q = (attackQueue*)malloc(sizeof(attackQueue));
    q->front = q->rear = NULL;
    return q;
}

attackQueue *appendNode(attackQueue *queue, attack *data) {
    if(queue == NULL){
        queue = createQueue();
    }

    attackNode* newNode = createNode(data);
    
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
        return queue;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;

    return queue;
}

attack* deleteNode(attackQueue *queue) {
    if (queue->front == NULL) {
        printf("Queue is empty!\n");
        return NULL; // Or any error code of your choice
    }
    attackNode* temp = queue->front;
    attack *value = temp->info;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return value;
}

attack *createAttack(collisionBox *box, int damage, int duration){
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