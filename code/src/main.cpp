#include "maze.h"
#include "navigation.h"
#include "motion.h"
#include "sensors.h"
#include "utils.h"
#include "flood_fill.h"

bool in_new_cell = true;
bool end = false;
Maze maze;
COORDINATES start = {0,0};
COORDINATES exit = {4,4};
PATH_STACK path_run1;
PATH_STACK path_run2;
ROTATION direction_to_rotation[5] = {HALF_TURN, LEFT, NO_TURN, RIGHT, HALF_TURN};
CARDINALS current_direction;

ROTATION calculate_turn(CARDINALS curr, CARDINALS target);

void setup(){
    Serial.begin(115200);
    delay(20);
    RESULT err = init_all_sensors();
    while(err){
        DEBBUG_PRINT(Serial.printf("Error occured in initialization: %s \n", error_table_translation[err]));
        delay(1000);
    }

    err = init_maze(&maze,start,exit);
    while(err){
        DEBBUG_PRINT(Serial.printf("Btw you need to give valid start and exit coordinates: %s \n", error_table_translation[err]));
        delay(1000);
    }

    err = init_stack(&path_run1);
    err = init_stack(&path_run2);

    current_direction = EAST;

}

void loop(){
    if (in_new_cell) {
        if (!end) {
            WALL_DIR new_walls[3];
            int len = 0;
            CARDINALS next_direction;
            RESULT rslt = detect_walls(new_walls, &len); //TODO: detect_walls
            //TODO: check rslt

            for (int i = 0; i<len; i++) {
                add_wall(&maze, new_walls[i]);  
            }

            rslt = one_iteration_flood_fill(&maze, &path_run1, &next_direction);
            //TODO: check rslt
            
            ROTATION rotation = calculate_turn(current_direction, next_direction);

            current_direction = next_direction;
        }
    }
}


ROTATION calculate_turn(CARDINALS curr, CARDINALS target){
    int diff = target - curr;

    if(diff > 2) diff -= 4;
    if (diff < -2) diff += 4;

    diff += 2;

    return direction_to_rotation[diff];
}

