#include "alien.h"

void initAliens(alien* aliens_list){
    for(int i = 0; i < MAX_ALIENS; i++)
        aliens_list[i].used = false;
}