#include "maze.h"
#include "navigation.h"
#include "motion.h"
#include "sensors.h"
#include "utils.h"
#include "flood_fill.h"
#include "gyroscope.h"
#include "run.h"
#include  <Wire.h>

#define CELL_LENGTH 180 //in mm

// Defined speeds for both run and search mode
#define MAX_SPEED_SEARCH 0.08
#define MAX_SPEED_RUN 0.16

int run1 = 1;
int run2 = 0;
int final_run = 1;
int end = 0;
Maze maze;
COORDINATES start_cell = {0,0};
COORDINATES exit_cell = {MAZE_SIZE-1,MAZE_SIZE-1};
PATH_STACK path_run1;
PATH_STACK path_run2;

CARDINALS current_direction;



#define BLOCK_ON_ERROR(error,message) \
    while(error){\
        message;\
        Serial.printf("Error: %s \n", error_table_translation[error]);\
        delay(1000); \
    } 

void setup(){
    Serial.begin(115200);
    delay(20);
    Wire.begin();
    delay(20);
    delay(5000);
    RESULT err = init_all_sensors();
    
    BLOCK_ON_ERROR(err, Serial.println("Error occured in initialization."));

    err = init_maze(&maze,start_cell,exit_cell);
    Serial.printf("mouse x : %d, mouse y : %d, end x: %d, end y : %d \n", maze.mouse_pos.x, maze.mouse_pos.y, maze.exit.x, maze.exit.y);
    BLOCK_ON_ERROR(err, Serial.println("Btw you need to give valid start and exit coordinates"));

    err = init_stack(&path_run1);
    BLOCK_ON_ERROR(err, Serial.println("Stack failed."));
    err = init_stack(&path_run2);
    BLOCK_ON_ERROR(err, Serial.println("Stack failed."));

    current_direction = START_ORIENTATION;

}

void loop(){
    if (run1) {
      Serial.println("code started");      
      //TODO peut etre qu'il faut update le gyro plus que ça
      WALL_DIR new_walls[3];
      int len = 0;
      CARDINALS next_direction;
      RESULT rslt = detect_walls(new_walls, &len, current_direction); 

      BLOCK_ON_ERROR(rslt, Serial.println("wall detection failed !!"));
      //TODO: check rslt
      for (int i = 0; i<len; i++) {
          Serial.println(new_walls[i]);
          rslt = add_wall(&maze, new_walls[i]);  
          BLOCK_ON_ERROR(rslt, Serial.println("add wall failed !!"));
      }
      Serial.println("add walls done");

      rslt = one_iteration_flood_fill(&maze, &path_run1, &next_direction);
      Serial.println("flood fill done");
      Serial.println(rslt);

      //BLOCK_ON_ERROR(rslt && rslt != MOUSE_END, Serial.println("flood fill failed !!"));

      if(rslt != MOUSE_END) {
          Serial.println(next_direction);
          ROTATION rotation = calculate_turn(current_direction, next_direction);

          current_direction = next_direction;

          rslt = turn(rotation, INPLACE);
          BLOCK_ON_ERROR(rslt, Serial.println("turn failed !!"));
          //TODO: check rslt
          delay(100);
          rslt = navigation_forward(CELL_LENGTH, MAX_SPEED_SEARCH);
          BLOCK_ON_ERROR(rslt, Serial.println("navigation forward failed !!"));
          //TODO: check rslt
      } else {
          run1 = 0;
          run2 = 1;
          maze.start = exit_cell;
          maze.exit = start_cell;
      }
    
    }
    else if(run2){
      Serial.println("code started");

      WALL_DIR new_walls[3];
      int len = 0;
      CARDINALS next_direction;
      RESULT rslt = detect_walls(new_walls, &len, current_direction); 

      BLOCK_ON_ERROR(rslt, Serial.println("wall detection failed !!"));
      //TODO: check rslt
      for (int i = 0; i<len; i++) {
          Serial.println(new_walls[i]);
          rslt = add_wall(&maze, new_walls[i]);  
          BLOCK_ON_ERROR(rslt, Serial.println("add wall failed !!"));
      }
      Serial.println("add walls done");

      rslt = one_iteration_flood_fill(&maze, &path_run2, &next_direction);
      Serial.println("flood fill done");
      Serial.println(rslt);


      //BLOCK_ON_ERROR(rslt && rslt != MOUSE_END, Serial.println("flood fill failed !!"));

      if(rslt != MOUSE_END) {
          Serial.println(next_direction);
          ROTATION rotation = calculate_turn(current_direction, next_direction);

          current_direction = next_direction;

          rslt = turn(rotation, INPLACE);
          BLOCK_ON_ERROR(rslt, Serial.println("turn failed !!"));
          //TODO: check rslt
          delay(100);
          rslt = navigation_forward(CELL_LENGTH, MAX_SPEED_SEARCH);
          BLOCK_ON_ERROR(rslt, Serial.println("navigation forward failed !!"));
          //TODO: check rslt
      } else {
          run2 = 0;
      }
    }
    else if(final_run){
      ROTATION rotation = calculate_turn(current_direction, START_ORIENTATION);
      RESULT rslt = turn(rotation, INPLACE);
      BLOCK_ON_ERROR(rslt, Serial.println("turn failed !!"));
      run(&path_run1, &path_run2, MAX_SPEED_RUN);
      final_run = 0;
    }

    for (int i = 0; i < 15; i++){
      update_gyro();
    }
  }


