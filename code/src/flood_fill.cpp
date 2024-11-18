#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;


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

char matrix[MAZE_SIZE][MAZE_SIZE];
typedef struct {
    int x;
    int y;
}COORDINATES;

typedef enum WALLS_DIR{LEFT, UP, RIGHT, DOWN};

typedef struct{
    unsigned char matrix[MAZE_SIZE][MAZE_SIZE];
    unsigned char walls[MAZE_SIZE][MAZE_SIZE];
    COORDINATES mouse_pos;
    COORDINATES exit;
    COORDINATES start;
}Maze;

Maze maze;
int dx[] = {0, -1, 0, 1};
int dy[] = {-1, 0, 1, 0};
int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};


void innit_maze(COORDINATES start, COORDINATES exit){
    maze.start = start;
    maze.exit = exit;
    maze.mouse_pos = start;
    std::memset(maze.walls, 0, MAZE_SIZE*MAZE_SIZE);
    std::memset(maze.matrix, 255, MAZE_SIZE*MAZE_SIZE);
}

void weighted_BFS(int x, int y, unsigned char currentDistance) {
    // If this cell's distance is already smaller, stop recursion
    if (maze.matrix[x][y] <= currentDistance) {
        return;
    }
    // Update the cell's distance
    maze.matrix[x][y] = currentDistance;

    // Explore all four directions
    for (int dir = 0; dir < 4; ++dir) {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        // Check boundaries
        if (!IN_BOUNDARIES(nx,ny))
            continue;

        // Check if there's a wall blocking this direction
        if (maze.walls[x][y] & WALLS[dir]){
            continue;
        }
        // Recur into the next cell
        weighted_BFS(nx, ny, currentDistance+1);
    }
}

#define EQUAL_COORD(coord_1, coord_2) (coord_1.x == coord_2.x && coord_1.y == coord_2.y)
#define INNIT_STACK(stack) stack.end = 0;

typedef struct {
    COORDINATES stack[MAZE_SIZE*MAZE_SIZE];
    size_t end;
}PATH_STACK;

void pop_n(size_t n, PATH_STACK* stack){
    stack->end -= n;
}

void push(COORDINATES new_coord, PATH_STACK* stack){
    for (size_t i = 0; i < stack->end; i++){
        if(EQUAL_COORD(stack->stack[i], new_coord)){
            pop_n(stack->end-i-1, stack);
            return;
        }
    }
    stack->stack[stack->end] = new_coord;
    stack->end++;
}




void display_walls(PATH_STACK* path){
  
    int n = MAZE_SIZE; // Size of the maze (assume square)

    for (int i = 0; i < n; ++i) {
        // Print the top walls of the row
        for (int j = 0; j < n; ++j) {
            cout << "+";
            if (maze.walls[i][j] & (1 << 1)) { // Check "Up" wall (Bit 1)
                cout << "---";
            } else {
                cout << "   ";
            }
        }
        cout << "+" << endl;

        // Print the left walls and spaces of the row
        for (int j = 0; j < n; ++j) {
            if (maze.walls[i][j] & (1 << 0)) { // Check "Left" wall (Bit 0)
                cout << "|";
            } else {
                cout << " ";
            }
            if(maze.mouse_pos.x == i && maze.mouse_pos.y == j){
                cout << " X ";
            }else{
                int in_stack = 0;
                for (size_t k = 0; k < path->end; k++){
                    COORDINATES coord = {i, j};
                    if(EQUAL_COORD(path->stack[k], coord)){
                        cout<< " O ";
                        in_stack = 1;
                    }    
                }
                if(!in_stack){
                    cout<< "   ";
                }
                
                //cout << setw(2 + 1)<< static_cast<int>(maze.matrix[i][j]); // Space inside the cell
            }

        }
        // Right boundary of the row
        cout << "|" << endl;
    }

    // Print the bottom boundary of the maze
    for (int j = 0; j < n; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

void add_wall(WALLS_DIR wall){
    unsigned char current_offset = 0;
    unsigned char next_cell_offset = 0;
    int nx = -1;
    int ny = -1;
    switch (wall){
        case LEFT:
            current_offset = LeftWall;
            next_cell_offset = RightWall;
            nx = maze.mouse_pos.x;
            ny = maze.mouse_pos.y-1;
            break;
        case UP:
            current_offset = UpWall;
            next_cell_offset = DownWall;
            nx = maze.mouse_pos.x-1;
            ny = maze.mouse_pos.y;
            break;
        case RIGHT:
            current_offset = RightWall;
            next_cell_offset = LeftWall;
            nx = maze.mouse_pos.x;
            ny = maze.mouse_pos.y+1;
            break;
        case DOWN:
            current_offset = DownWall;
            next_cell_offset = UpWall;
            nx = maze.mouse_pos.x+1;
            ny = maze.mouse_pos.y;
            break;
        }
    maze.walls[maze.mouse_pos.x][maze.mouse_pos.y] |= current_offset;
    if(0 <= nx && nx < MAZE_SIZE && 0 <= ny && ny < MAZE_SIZE){
        maze.walls[nx][ny] |= next_cell_offset;
    }
}



void run_flood_fill(PATH_STACK* path_stack, char true_walls[][MAZE_SIZE]){
    push(maze.mouse_pos, path_stack);
    while(maze.mouse_pos.x != maze.exit.x || maze.mouse_pos.y != maze.exit.y){

        // fake discovery of the new walls
        unsigned char new_walls =  true_walls[maze.mouse_pos.x][maze.mouse_pos.y];
        for (size_t i = 0; i < 4; i++){
            if(WALLS[i]& new_walls){
                add_wall(static_cast<WALLS_DIR>(i));
            }
        }

        DEBBUG_PRINT(display_walls(path_stack));
        DEBBUG_PRINT(cout<<"\n";);
        // set to 0
        std::memset(maze.matrix, 255, MAZE_SIZE*MAZE_SIZE);
        weighted_BFS(maze.exit.x, maze.exit.y, 0);
        unsigned char current_min = 255;
        int min_dir = 0;
        for (size_t i = 0; i < 4; i++){
            if(WALLS[i]& new_walls){
                continue;
            }
            if(current_min > maze.matrix[maze.mouse_pos.x+dx[i]][maze.mouse_pos.y+dy[i]]){
                current_min = maze.matrix[maze.mouse_pos.x+dx[i]][maze.mouse_pos.y+dy[i]];
                min_dir = i;
            }
        }

        maze.mouse_pos.x += dx[min_dir];
        maze.mouse_pos.y += dy[min_dir];
        push(maze.mouse_pos, path_stack);
    }
}


int main(int argc, char const *argv[]){
    COORDINATES start = {0,0};
    COORDINATES exit = {7,7};
    innit_maze(start, exit);
 
    // char true_walls[MAZE_SIZE][MAZE_SIZE] = {
    //     {0b1011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

    //     {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},

    //     {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

    //     {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},

    //     {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

    //     {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},

    //     {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

    //     {0b1011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100}, 

    // };


    // 2 solution maze
    char true_walls[MAZE_SIZE][MAZE_SIZE] = 
            {{0b0011, 0b1010 , 0b0110 , 0b1011 ,0b1010 ,0b0010 ,0b1010 ,0b0110},

                {0b0101, 0b1011 , 0b1000 , 0b1010 ,0b1010 ,0b1100 ,0b0111 ,0b0101},

                {0b0101, 0b0011 , 0b0110 , 0b1011 ,0b0010 ,0b0010 ,0b0100 ,0b0101},

                {0b0101, 0b0101 , 0b1001 , 0b0110 ,0b0101 ,0b0101 ,0b1101 ,0b0101},

                {0b0101, 0b1001 , 0b0110 , 0b0101 ,0b0101 ,0b1001 ,0b1010 ,0b1100},

                {0b0001, 0b0110 , 0b0101 , 0b0101 ,0b1001 ,0b0110 ,0b0011 ,0b0110},

                {0b0101, 0b1101 , 0b0101 , 0b1001 ,0b0110 ,0b1001 ,0b1100 ,0b0101},

                {0b1001, 0b1010 , 0b1000 , 0b1110 ,0b1001 ,0b1010 ,0b1010 ,0b1100}};

    // stuck 1 solution maze
    // char true_walls[MAZE_SIZE][MAZE_SIZE] = 
    //         {{0b0011, 0b1010 , 0b0110 , 0b1011 ,0b1010 ,0b0010 ,0b1010 ,0b0110},

    //         {0b0101, 0b1011 , 0b1000 , 0b1010 ,0b1010 ,0b1100 ,0b0111 ,0b0101},

    //         {0b0101, 0b0011 , 0b0110 , 0b1011 ,0b0110 ,0b0011 ,0b0100 ,0b0101},

    //         {0b0101, 0b0101 , 0b1001 , 0b0110 ,0b0101 ,0b0101 ,0b1101 ,0b0101},

    //         {0b0101, 0b1001 , 0b0110 , 0b0101 ,0b0101 ,0b1001 ,0b1010 ,0b1100},

    //         {0b0001, 0b0110 , 0b0101 , 0b0101 ,0b1001 ,0b0110 ,0b0011 ,0b0110},

    //         {0b0101, 0b1101 , 0b0101 , 0b1001 ,0b0110 ,0b1001 ,0b1100 ,0b0101},

    //         {0b1001, 0b1010 , 0b1000 , 0b1110 ,0b1001 ,0b1010 ,0b1010 ,0b1100}};

    PATH_STACK path1;
    PATH_STACK path2;
    INNIT_STACK(path1);
    INNIT_STACK(path2);

    run_flood_fill(&path1, true_walls);
    //invert the maze
    maze.exit = start;
    maze.start = exit;
    cout<< "First run : \n";
    display_walls(&path1);

    run_flood_fill(&path2, true_walls);

    cout<< "Return path: \n";
    display_walls(&path2);
    return 0;
}



