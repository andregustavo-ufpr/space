#include <stdio.h>
#include <string.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>																																					//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include "entity.h"
#include "attack.h"

#define FPS 30

#define MOVEMENT_SPEED 5
#define EXPIRATION 50
#define MAIN_PLAYER_HEALTH 200
#define MAIN_PLAYER_ATTACK 20
#define BASE_ATTACK_SPEED 20

attackQueue *head;

void pauseGame(int *paused) {
    if(!*paused) *paused = 1;
    else *paused = 0;
}

void end_game(ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue) {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

entity *initMainPlayer(){
    collisionBox *player_box = createBox(20, Y_SCREEN/2, 50, 80);
    entity *main_player = createEntity(player_box, MAIN_PLAYER_HEALTH);

    if(main_player == NULL){
        printf("Failed to create new player");
        return NULL;
    }
    
    return main_player;
}

void handleAttack(entity *attacker){
    collisionBox *attack_box = createBox(attacker->box->coord->x + attacker->box->width, attacker->box->coord->y, 10, 10);
    
    addVelocity(BASE_ATTACK_SPEED, attack_box->coord, X_AXIS);
    
    attack *new_attack = createAttack(attack_box, MAIN_PLAYER_ATTACK, EXPIRATION);

    head = appendNode(head, new_attack);
}

void handleMovement(int keycode, position *coord, int invert){
    int direction = 1;
    int axis = X_AXIS;

    switch(keycode){
        case ALLEGRO_KEY_A:
            direction = -1;
            break;
        case ALLEGRO_KEY_W:
            direction = -1;
            axis = Y_AXIS;
            break;
        case ALLEGRO_KEY_S:
            axis = Y_AXIS;
            break;
        case ALLEGRO_KEY_D:
        default:
            break;
    }

    addVelocity(MOVEMENT_SPEED*direction*(invert == 1 ? -1 : 1), coord, axis);
}

void drawAttacks(){
    if(head != NULL){
        attackNode *pointer = head->front;
        
        while(pointer != NULL){
            updatePos(pointer->info->box->coord);
            al_draw_filled_rectangle(pointer->info->box->coord->x, pointer->info->box->coord->y, pointer->info->box->coord->x +pointer->info->box->width, pointer->info->box->coord->y -pointer->info->box->height, al_map_rgb(0, 0, 0));
            pointer->info->expire_time -= 1;
            
            if(pointer->info->expire_time <= 0){
                deleteNode(head);
            }

            if(pointer != NULL){
                pointer = pointer->next;
            }
            else{
                break;
            }
        }
    }
}

void drawMainPlayer(collisionBox *box){
    al_draw_filled_triangle(box->coord->x, box->coord->y - box->height/2, box->coord->x + box->width, box->coord->y, box->coord->x, box->coord->y + box->height/2, al_map_rgb(255, 0, 0));
}

int main() {

    // Initializers
    al_init();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();

    // Game config logic
    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;

    queue = al_create_event_queue();
    display = al_create_display(X_SCREEN, Y_SCREEN);
    timer = al_create_timer(1.0/FPS);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    int running = 1, paused = 0;

    entity *main_player = initMainPlayer();
    while (running == 1){

        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if(!paused) {
            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = 0;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                        pauseGame(&paused);
                        // Draw menu
                        al_clear_to_color(al_map_rgb(0,0,0));
                        al_flip_display();
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_W || 
                        event.keyboard.keycode == ALLEGRO_KEY_S || 
                        event.keyboard.keycode == ALLEGRO_KEY_D || 
                        event.keyboard.keycode == ALLEGRO_KEY_A) 
                    {
                        handleMovement(event.keyboard.keycode, main_player->box->coord, 0);
                    }
                    if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                        handleAttack(main_player);
                    }
                    break;
                case ALLEGRO_EVENT_KEY_UP:
                    if(event.keyboard.keycode == ALLEGRO_KEY_W || 
                        event.keyboard.keycode == ALLEGRO_KEY_S || 
                        event.keyboard.keycode == ALLEGRO_KEY_D || 
                        event.keyboard.keycode == ALLEGRO_KEY_A) 
                    {
                        handleMovement(event.keyboard.keycode, main_player->box->coord, 1);
                    }
                    break;
                case ALLEGRO_EVENT_TIMER:
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    updatePos(main_player->box->coord);
				    
                    drawMainPlayer(main_player->box);
                    drawAttacks();

                    al_flip_display();

                    break;
                default:
                    break;
            }
        }
        else {
            al_clear_to_color(al_map_rgba(0, 0, 0, 0.3));
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                pauseGame(&paused);
            }
        }
    }

    end_game(timer, display, queue);

    return 0;
}