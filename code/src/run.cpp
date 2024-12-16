#include "run.h"
#include "navigation.h"



// Maximum 2 instructions per cell
#define INSTRUCTION_STACK_SIZE 2*MAZE_SIZE*MAZE_SIZE

typedef enum {NAVIGATION_FORWARD, NAVIGATION_TURN}INSTRUCTION_TYPE;


typedef struct {
    float distance;
    float speed;
}NAVIGATION_FORWARD_ARGS;

typedef struct {
    ROTATION instruction;
    MODE mode;
}NAVIGATION_TURN_ARGS;

typedef union{
    NAVIGATION_FORWARD_ARGS forward_args;
    NAVIGATION_TURN_ARGS turn_args;
}RUN_ARGS;

typedef union {
    RESULT (*instruction_forward)(float , float);
    RESULT (*instruction_turn)(ROTATION, MODE);
}RUN_INSTRUCTION;


typedef struct{
    INSTRUCTION_TYPE type;
    RUN_INSTRUCTION instruction;
    RUN_ARGS args;
}STEP;



static RESULT reverse_stack(PATH_STACK* stack){
    CHECK_AND_THROW(!stack, NULL_PTR);

    PATH_STACK out_stack;   
    RESULT err = init_stack(&out_stack);
    size_t stack_end = stack->end; 


    // The following is not securely satisfying but it removes the loop check that would be done
    //if we pushed each element one by one. WARNING: THIS higly depends on paths stack implementation.
    out_stack.end = stack_end;
    for (size_t i = 0; i < stack_end; i++){
        out_stack.stack[i] = stack->stack[(stack_end-1) - i];
    }
    *stack = out_stack;
    return NO_ERROR;
}



//// Instruction stack ////

typedef struct{
    size_t end;
    STEP step_stack[INSTRUCTION_STACK_SIZE];
}INSTRUCTION_STACK;

static RESULT init_instruction_stack(INSTRUCTION_STACK* stack){
    CHECK_AND_THROW(!stack, NULL_PTR);
    stack->end = 0;
    return NO_ERROR;
}

static RESULT push(INSTRUCTION_STACK* stack, STEP step){
    CHECK_AND_THROW(!stack, NULL_PTR);
    CHECK_AND_THROW(stack->end >= INSTRUCTION_STACK_SIZE, STACK_OVERFLOW);

    stack->step_stack[stack->end] = step;
    stack->end++;

    return NO_ERROR;
}

static RESULT pop(INSTRUCTION_STACK* stack, STEP* out){
    CHECK_AND_THROW(!stack, NULL_PTR);
    CHECK_AND_THROW(!(stack->end > 0), STACK_OVERFLOW);
    
    if(out){
        *out = stack->step_stack[stack->end-1];
    }

    stack->end--;
    return NO_ERROR;
}
//// end Instruction stack ////

static RESULT follow_instructions(INSTRUCTION_STACK* instruction_stack){
    CHECK_AND_THROW(!instruction_stack, NULL_PTR);
    RESULT err = NO_ERROR;
    for (size_t i = 0; i < instruction_stack->end; i++){
        STEP curent_step = instruction_stack->step_stack[i];
        switch (curent_step.type){

        case NAVIGATION_FORWARD:
            curent_step.args.forward_args;
            err = curent_step.instruction.instruction_forward(curent_step.args.forward_args.distance, curent_step.args.forward_args.speed);
            break;
        
        case NAVIGATION_TURN:
            err = curent_step.instruction.instruction_turn(curent_step.args.turn_args.instruction, curent_step.args.turn_args.mode);
            break;
        default:
            break;
        }
        PROPAGATE_ERROR(err);
    }
    
    return NO_ERROR;
}

static RESULT compute_instructions_stack(PATH_STACK* run_path, float run_speed, INSTRUCTION_STACK* out){
    CHECK_AND_THROW(!run_path, NULL_PTR);
    CHECK_AND_THROW(!out, NULL_PTR);
    RESULT err = init_instruction_stack(out);
    PROPAGATE_ERROR(err);

    INSTRUCTION_STACK instruction_stack;
    err = init_instruction_stack(&instruction_stack);
    PROPAGATE_ERROR(err);

    CARDINALS current_direction = START_ORIENTATION;
    CARDINALS next_direction;
    int current_dx = 0;
    int current_dy = 1;
    int next_dx = 0;
    int next_dy = 0;
    COORDINATES current_cell;
    COORDINATES next_cell;

    for (size_t index_in_path = 0; index_in_path < (run_path->end-1); index_in_path++){

        current_cell = run_path->stack[index_in_path];
        next_cell = run_path->stack[index_in_path+1];

        next_dx = next_cell.x - current_cell.x;
        next_dy = next_cell.y - current_cell.y;

        if (next_dx == 0) {
            if (next_dy == -1) {
                next_direction = WEST;
            }
            else if (next_dy == 1) {
                next_direction = EAST;
            }
        }
        else if (next_dy == 0) {
            if (next_dx == -1) {
                next_direction = NORTH;
            }
            else if (next_dx == 1) {
                next_direction = SOUTH;
            }
        }

        ROTATION rotation = calculate_turn(current_direction, next_direction);  
        float forward_distance = CELL_SIZE;

        // avoid the first iteration 
        if(rotation == NO_TURN && index_in_path != 0){
            STEP last_forward;
            // the last step is always a forward
            err = pop(&instruction_stack, &last_forward);
            PROPAGATE_ERROR(err);
            forward_distance = last_forward.args.forward_args.distance + CELL_SIZE;
        }else{
            // create the turn instruction
            RUN_INSTRUCTION function;
            function.instruction_turn = turn;
            NAVIGATION_TURN_ARGS turn_args = {rotation, INPLACE};
            RUN_ARGS args;
            args.turn_args = turn_args;
            STEP turn_step = {NAVIGATION_TURN, function, args};

            err = push(&instruction_stack, turn_step);
            PROPAGATE_ERROR(err);
        }
            // create the forward instruction
        RUN_INSTRUCTION function;
        function.instruction_forward = navigation_forward;
        NAVIGATION_FORWARD_ARGS forward_args = {forward_distance, run_speed};
        RUN_ARGS args;
        args.forward_args = forward_args;
        STEP forward_step  = {NAVIGATION_FORWARD, function, args};
        

        err = push(&instruction_stack, forward_step);
        PROPAGATE_ERROR(err);

        current_direction = next_direction;
    }
    *out = instruction_stack;
    return NO_ERROR;
}


RESULT run(PATH_STACK* path_1, PATH_STACK* path_2, float run_speed){
    CHECK_AND_THROW(!path_1, NULL_PTR);
    CHECK_AND_THROW(!path_2, NULL_PTR);
    INSTRUCTION_STACK stack_1;
    INSTRUCTION_STACK stack_2;

    RESULT err = compute_instructions_stack(path_1, run_speed, &stack_1);
    PROPAGATE_ERROR(err);
    err = compute_instructions_stack(path_2, run_speed, &stack_2);
    PROPAGATE_ERROR(err);
    

    err = follow_instructions((stack_1.end < stack_2.end)? &stack_1: &stack_2);
    PROPAGATE_ERROR(err);
    
    return NO_ERROR;
}

