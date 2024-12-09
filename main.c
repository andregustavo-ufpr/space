#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>																																					//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>																																											//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>
#include <allegro5/events.h>
#include "entity.h"
#include "attack.h"
#include "alien.h"

#define FPS 30

// TODO:
// Different kind of attacks
// Implement Collectables as different attacks
// Menu

#define MOVEMENT_SPEED 5
#define EXPIRATION 50
#define MAIN_PLAYER_HEALTH 40
#define MAIN_PLAYER_DAMAGE 20
#define BASE_ATTACK_SPEED 20
#define MAX_ATTACKS 120

#define NORMAL_ATTACK 1
#define NORMAL_H 10
#define NORMAL_W 10
#define SHOTGUN 2
#define LASER 3

attack attack_list[MAX_ATTACKS] = {};
entity *main_player;

long event_timer = 0;
long score = 0;
unsigned int phase = 0;
unsigned int enemies_killed = 0;
unsigned int enemies_spawned = 0;

const int ALIEN_W[] = { 21, 22, 22, 68 };
const int ALIEN_H[] = { 14, 15, 15, 41};
const int ALIEN_V[] = { 5, 10, 8, 2};
const int ALIEN_HEALTH[] = {40, 20, 20, 120};

bool can_spawn_powerup = true;

// General Init

void mustInit(bool test, const char *description){
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int between(int low, int high){
    return low + (rand() % (high - low));
}

void togglePause(int *paused) {
    if(!*paused) *paused = 1;
    else *paused = 0;
}

void end_game(ALLEGRO_TIMER *timer, ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *queue) {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}

// Sprites

#define SHIP_W 12
#define SHIP_H 13

#define SHIP_SHOT_W 2
#define SHIP_SHOT_H 9

#define LIFE_W 6
#define LIFE_H 6

#define ALIEN_SHOT_W 4
#define ALIEN_SHOT_H 4

#define EXPLOSION_FRAMES 4
#define SPARKS_FRAMES    3

typedef struct SPRITES
{
    ALLEGRO_BITMAP* _sheet;

    ALLEGRO_BITMAP* ship;
    ALLEGRO_BITMAP* life;

    ALLEGRO_BITMAP* alien[4];
    ALLEGRO_BITMAP* alien_shot;

    ALLEGRO_BITMAP* explosion[EXPLOSION_FRAMES];
    ALLEGRO_BITMAP* sparks[SPARKS_FRAMES];
} SPRITES;
SPRITES sprites;

ALLEGRO_BITMAP* grabSprite(int x, int y, int w, int h) {
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(sprites._sheet, x, y, w, h);
    mustInit(sprite, "sprite grab");
    return sprite;
}

void initSprites() {
    sprites._sheet = al_load_bitmap("spritesheet.png");
    mustInit(sprites._sheet, "spritesheet");

    sprites.alien[0] = grabSprite(19, 0, 14, 9);
    sprites.alien[1] = grabSprite(19, 10, 13, 10);
    sprites.alien[2] = grabSprite(0, 49, 9, 12);
    sprites.alien[3] = grabSprite(0, 21, 45, 27);

    sprites.alien_shot = grabSprite(13, 10, ALIEN_SHOT_W, ALIEN_SHOT_H);

    sprites.explosion[0] = grabSprite(33, 10, 9, 9);
    sprites.explosion[1] = grabSprite(43, 9, 11, 11);
    sprites.explosion[2] = grabSprite(46, 21, 17, 18);
    sprites.explosion[3] = grabSprite(46, 40, 17, 17);

    sprites.sparks[0] = grabSprite(34, 0, 10, 8);
    sprites.sparks[1] = grabSprite(45, 0, 7, 8);
    sprites.sparks[2] = grabSprite(54, 0, 9, 8);
}

void sprites_deinit()
{

    al_destroy_bitmap(sprites.alien[0]);
    al_destroy_bitmap(sprites.alien[1]);
    al_destroy_bitmap(sprites.alien[2]);

    al_destroy_bitmap(sprites.sparks[0]);
    al_destroy_bitmap(sprites.sparks[1]);
    al_destroy_bitmap(sprites.sparks[2]);

    al_destroy_bitmap(sprites.explosion[0]);
    al_destroy_bitmap(sprites.explosion[1]);
    al_destroy_bitmap(sprites.explosion[2]);
    al_destroy_bitmap(sprites.explosion[3]);

    al_destroy_bitmap(sprites._sheet);
}

// Main Player
entity *initMainPlayer(){
    collisionBox *player_box = createBox(20, Y_SCREEN/2, 25, 40);
    entity *main_player = createEntity(player_box, MAIN_PLAYER_HEALTH, NORMAL_ATTACK);
    main_player->attack_timer = 0;
    if(main_player == NULL){
        printf("Failed to create new player");
        return NULL;
    }
    
    return main_player;
}

void drawMainPlayer(collisionBox *box){
    al_draw_filled_triangle(
        box->coord->x, 
        box->coord->y - box->height/2, 
        box->coord->x + box->width, 
        box->coord->y, 
        box->coord->x, 
        box->coord->y + box->height/2, 
        al_map_rgb(255, 0, 0)
    );
}

void updateMainPlayer(entity *main_player){
    updatePos(main_player->box->coord); 
    if(main_player->attack_timer > 0){
        main_player->attack_timer -= 1;
    }
    drawMainPlayer(main_player->box);
}


// Attacks
void initAttacks(){
    for(int i = 0; i < MAX_ATTACKS; i++)
        attack_list[i].used = false;
}

void handleAttack(entity *attacker){

    if(attacker->attack_timer == 0){

        attacker->attack_timer = 15;

        if(attacker->attack_type == NORMAL_ATTACK){
            collisionBox *attack_box = createBox(attacker->box->coord->x + attacker->box->width, attacker->box->coord->y, 10, 10);
            
            addVelocity(BASE_ATTACK_SPEED, attack_box->coord, X_AXIS);

            for(int i = 0; i < MAX_ATTACKS; i++){
            
                if(attack_list[i].used)
                    continue;

                attack_list[i].box = attack_box;
                attack_list[i].used = true;
                attack_list[i].damage = MAIN_PLAYER_DAMAGE;
                attack_list[i].expire_time = EXPIRATION;
                attack_list[i].owner = PLAYER;

                return;
            }
        }

        if(attacker->attack_type == SHOTGUN){

            int spawned = 0;

            for(int i = 0; i < MAX_ATTACKS; i++){
            
                if(attack_list[i].used)
                    continue;

                collisionBox *attack_box = NULL;

                switch (spawned){
                    case 0:
                        attack_box = createBox(attacker->box->coord->x + attacker->box->width, attacker->box->coord->y, 10, 10);
                        addVelocity(BASE_ATTACK_SPEED, attack_box->coord, X_AXIS);
                        addVelocity(-2, attack_box->coord, Y_AXIS);
                        break;
                    case 1:
                        attack_box = createBox(attacker->box->coord->x + attacker->box->width, attacker->box->coord->y, 10, 10);
                        addVelocity(BASE_ATTACK_SPEED, attack_box->coord, X_AXIS);
                        break;
                    case 2:
                        attack_box = createBox(attacker->box->coord->x + attacker->box->width, attacker->box->coord->y, 10, 10);
                        addVelocity(BASE_ATTACK_SPEED, attack_box->coord, X_AXIS);
                        addVelocity(2, attack_box->coord, Y_AXIS);
                        break;
                    
                    default:
                        break;
                }

                attack_list[i].box = attack_box;
                attack_list[i].used = true;
                attack_list[i].damage = MAIN_PLAYER_DAMAGE;
                attack_list[i].expire_time = EXPIRATION;

                spawned ++;

                if(spawned >= 3){
                    return;
                }
            }
        }
    }
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
    for(int i = 0; i < MAX_ATTACKS; i++){
        if(!attack_list[i].used)
            continue;
        
        updatePos(attack_list[i].box->coord);
        al_draw_filled_rectangle(
            attack_list[i].box->coord->x, 
            attack_list[i].box->coord->y, 
            attack_list[i].box->coord->x + attack_list[i].box->width, 
            attack_list[i].box->coord->y - attack_list[i].box->height, 
            al_map_rgb(6, 229, 73)
        );
        
        // checkCollision()
        
        attack_list[i].expire_time -= 1;
        
        if(attack_list[i].expire_time <= 0){
            attack_list[i].used = false;
            free(attack_list[i].box);
        }
    }
}

#define MAX_ALIENS 20
alien enemies[MAX_ALIENS];

void enemiesInit() {
    for(int i = 0; i < MAX_ALIENS; i++)
        enemies[i].used = false;
}

void handleEnemyDeath(alien *enemy){
    switch(enemy->type) {
        case ALIEN_TYPE_BUG:
            score += 200;
            break;
        case ALIEN_TYPE_ARROW:
            score += 150;
            break;
        case COLLECTABLE:
            main_player->attack_type = SHOTGUN;
            score += 100;
            can_spawn_powerup = true;
            break;
        case ALIEN_TYPE_BIG:
            score += 800;
            break;
        default:
            break;
    }

    enemy->used = false;
}


void createAlien(alien *enemy){
    if(phase == 1){
        enemy->type = between(ALIEN_TYPE_BUG, ALIEN_TYPE_BIG);
    }
    else{
        enemy->type = between(ALIEN_TYPE_ARROW, ALIEN_TYPE_N);
    }
    enemy->shot_timer = between(1, 99);
    enemy->used = true;

    if((!can_spawn_powerup || main_player->attack_type == SHOTGUN) && enemy->type == COLLECTABLE){
        enemy->type = ALIEN_TYPE_ARROW;
    }

    if(enemy->type == COLLECTABLE){
        can_spawn_powerup = false;
    }

    collisionBox *box = createBox(
        X_SCREEN, 
        (Y_SCREEN + between(-Y_SCREEN/2, Y_SCREEN/2))/2, 
        ALIEN_H[enemy->type], 
        ALIEN_W[enemy->type]
    );

    addVelocity(-(ALIEN_V[enemy->type]), box->coord, X_AXIS);

    enemy->health = ALIEN_HEALTH[enemy->type];

    enemy->box = box;
}

void alienAttack(alien attacker){
    if(attacker.shot_timer == 0){

        attacker.shot_timer = 15;

        collisionBox *attack_box = createBox(attacker.box->coord->x + attacker.box->width, attacker.box->coord->y, 20, 20);
        
        addVelocity(-15, attack_box->coord, X_AXIS);

        for(int i = 0; i < MAX_ATTACKS; i++){
        
            if(attack_list[i].used)
                continue;

            attack_list[i].box = attack_box;
            attack_list[i].used = true;
            attack_list[i].damage = MAIN_PLAYER_DAMAGE;
            attack_list[i].expire_time = EXPIRATION;
            attack_list[i].owner = ALIEN;

            return;
        }
    }
}

void updateAliens(alien* aliens_list) {
    int new_quota = (event_timer % 120) ? 0 : between(2, 5);

    if(enemies_spawned >= 20 && score > 3000){
        new_quota = 0;
    }
    
    for(int i = 0; i < MAX_ALIENS; i++) {

        if(!aliens_list[i].used) {
            if(new_quota > 0) {
                createAlien(&aliens_list[i]);
                new_quota--;
                enemies_spawned += 1;
            }
            continue;
        }

        updatePos(aliens_list[i].box->coord); 

        if(aliens_list[i].box->coord->x <= 0){
            aliens_list[i].used = false;
            if(aliens_list[i].type == COLLECTABLE){
                can_spawn_powerup = true;
            }
            continue;
        }

        for(int j = 0; j < MAX_ATTACKS; j++){

            if(!attack_list[j].used){
                continue;
            }

            if(checkCollision(aliens_list[i].box, attack_list[j].box) && attack_list[j].owner != ALIEN){
                aliens_list[i].health -= attack_list[j].damage;
                attack_list[j].used = false;
            }
        }

        // int cx = aliens_list[i].box->coord->x + (ALIEN_W[aliens_list[i].type] / 2);
        // int cy = aliens_list[i].box->coord->y + (ALIEN_H[aliens_list[i].type] / 2);

        if(aliens_list[i].health <= 0){
            // fx_add(false, cx, cy);

            handleEnemyDeath(&aliens_list[i]);
            enemies_killed += 1;
            continue;
        }

        aliens_list[i].shot_timer--;
        if(aliens_list[i].shot_timer == 0) {
            switch(aliens_list[i].type) {
                case ALIEN_TYPE_BIG:
                    alienAttack(aliens_list[i]);
                    aliens_list[i].shot_timer = 200;
                    break;
                default:
                    break;
            }
        }
    }
}


void drawAliens(alien *aliens_list){
    for(int i = 0; i < MAX_ALIENS; i++) {
        if(!aliens_list[i].used)
            continue;
        
        al_draw_bitmap(
            sprites.alien[aliens_list[i].type], 
            aliens_list[i].box->coord->x, 
            aliens_list[i].box->coord->y,
            0
        );
    }
}

ALLEGRO_FONT* font;
long score_display;

void initHUD() {
    font = al_create_builtin_font();
    mustInit(font, "font");

    score_display = 0;
}

void destroyHUD() {
    al_destroy_font(font);
}

void updateHUD() {
    if(event_timer % 2)
        return;

    for(long i = 5; i > 0; i--) {
        long diff = 1 << i;
        if(score_display <= (score - diff))
            score_display += diff;
    }
}

void drawHUD() {
    al_draw_textf(
        font,
        al_map_rgb_f(1,1,1),
        1, 1,
        0,
        "%06ld",
        score_display
    );

    if(main_player->health <= 0){
        al_draw_text(
            font,
            al_map_rgb_f(1,1,1),
            X_SCREEN / 2, Y_SCREEN / 2,
            ALLEGRO_ALIGN_CENTER,
            "G A M E  O V E R"
        );
    }
}

void checkPlayerColision(){
    for(int i = 0; i< MAX_ATTACKS; i++){
        if(!attack_list[i].used){
            continue;
        }

        if(checkCollision(main_player->box, attack_list[i].box) && attack_list[i].owner != PLAYER){
            main_player->health -= attack_list[i].damage;
            attack_list[i].used = false;
            continue;
        }
    }
}

int destroyAttacks(){
    for(int i = 0; i < MAX_ATTACKS; i ++){
        if(attack_list[i].box != NULL){
            destroyBox(attack_list[i].box);
        }
    }

    return 1;
}

int destroyAliens(){
    for(int i = 0; i < MAX_ATTACKS; i ++){
        if(enemies[i].box != NULL){
            destroyBox(attack_list[i].box);
        }
    }

    return 1;
}

int main() {

    // Initializers
    mustInit(al_init(), "allegro");
    mustInit(al_install_keyboard(), "keyboard");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    mustInit(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    mustInit(queue, "queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);
    mustInit(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    mustInit(font, "font");

    mustInit(al_init_image_addon(), "image addon");
    initSprites();

    initHUD();

    mustInit(al_init_primitives_addon(), "primitives");

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    int running = 1, paused = 0;

    main_player = initMainPlayer();
    initAttacks();
    initAliens(enemies);

    phase = 1;
    int wait_time = 0;

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
                        togglePause(&paused);
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
                    if(wait_time > 0){
                        wait_time--;
                        
                        if(phase == 2){
                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, Y_SCREEN/2,
                                0,
                                "PHASE 2"
                            );
                        }
                        else{
                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, Y_SCREEN/2,
                                0,
                                "Y O U  W I N"
                            );

                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, (Y_SCREEN/2 + 50),
                                0,
                                "Score: %ld",
                                score
                            );
                        }

                        continue;
                    }
                    // spawnEnemies();
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    
                    updateMainPlayer(main_player);
                    updateAliens(enemies);
                    updateHUD();

                    drawAttacks();
                    drawAliens(enemies);
                    drawHUD();

                    checkPlayerColision();
                    event_timer += 1;

                    if(phase == 1){
                        if(score > 3000){
                            phase = 2;
                            enemies_killed = 0;
                            enemies_spawned = 0;
                            score = 0;
                            score_display = 0;
                            
                            for(int i=0; i< MAX_ALIENS; i++){
                                enemies[i].used = false;
                            }

                            for(int i=0; i< MAX_ATTACKS; i++){
                                attack_list[i].used = false;
                            }

                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, Y_SCREEN/2,
                                0,
                                "PHASE 2"
                            );

                            wait_time = 60;
                        }
                    }
                    if(phase == 2){
                        if(score > 4000){
                            enemies_killed = 0;
                            enemies_spawned = 0;
                            
                            for(int i=0; i< MAX_ALIENS; i++){
                                enemies[i].used = false;
                            }

                            for(int i=0; i< MAX_ATTACKS; i++){
                                attack_list[i].used = false;
                            }

                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, Y_SCREEN/2,
                                0,
                                "Y O U  W I N"
                            );

                            al_draw_textf(
                                font,
                                al_map_rgb(255,255,255),
                                X_SCREEN/2, (Y_SCREEN/2 + 50),
                                0,
                                "Score: %ld",
                                score
                            );

                            wait_time = 300;
                        }
                    }

                    al_flip_display();

                    break;
                default:
                    break;
            }
        }
        else {
            al_clear_to_color(al_map_rgba(0, 0, 0, 0.3));
            if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                togglePause(&paused);
            }
        }
    }

    end_game(timer, disp, queue);
    destroyHUD();
    destroyAliens();
    destroyAttacks();

    return 0;
}