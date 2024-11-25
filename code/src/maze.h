#define MAZE_SIZE 8
#define DEBBUG 1
#define DEBBUG_PRINT(SENTENCE) \
    do{\
        if(DEBBUG){\
            SENTENCE;\
        }\
    }while(0)\
    

#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8

#define IN_BOUNDARIES(X,Y) (0 <= X && X < MAZE_SIZE && 0 <= Y && Y < MAZE_SIZE)

#define EQUAL_COORD(coord_1, coord_2) (coord_1.x == coord_2.x && coord_1.y == coord_2.y)
#define INIT_STACK(stack) stack.end = 0;

typedef struct {
    int x;
    int y;
}COORDINATES;

typedef struct {
    COORDINATES stack[MAZE_SIZE*MAZE_SIZE];
    size_t end;
}PATH_STACK;

typedef struct{
    unsigned char matrix[MAZE_SIZE][MAZE_SIZE];
    unsigned char walls[MAZE_SIZE][MAZE_SIZE];
    COORDINATES mouse_pos;
    COORDINATES exit;
    COORDINATES start;
}Maze;

typedef enum WALL_DIR{LEFT, UP, RIGHT, DOWN};

void pop_n(size_t n, PATH_STACK* stack); // À mettre dans utils ou maze ?

void push(COORDINATES new_coord, PATH_STACK* stack); // même question

void init_maze(COORDINATES start, COORDINATES exit);

void run_maze();

void search_maze();

void display_walls(PATH_STACK* path); // À inclure ici ou dans un éventuel ui.h ? ou nulle part :P

void add_wall(WALL_DIR wall);