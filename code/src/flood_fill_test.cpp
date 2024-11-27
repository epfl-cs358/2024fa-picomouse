#include <flood_fill.h>
#include <utils.h>
#include <iostream>

using namespace std;

void display_walls(PATH_STACK* path, Maze* maze){
  
    int n = MAZE_SIZE; // Size of the maze (assume square)

    for (int i = 0; i < n; ++i) {
        // Print the top walls of the row
        for (int j = 0; j < n; ++j) {
            cout << "+";
            if (maze->walls[i][j] & (1 << 1)) { // Check "Up" wall (Bit 1)
                cout << "---";
            } else {
                cout << "   ";
            }
        }
        cout << "+" << endl;

        // Print the left walls and spaces of the row
        for (int j = 0; j < n; ++j) {
            if (maze->walls[i][j] & (1 << 0)) { // Check "Left" wall (Bit 0)
                cout << "|";
            } else {
                cout << " ";
            }
            if(maze->mouse_pos.x == i && maze->mouse_pos.y == j){
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


int main(int argc, char const *argv[]){
    Maze maze;
    COORDINATES start = {0,0};
    COORDINATES exit = {7,7};
    innit_maze(&maze, start, exit);
 
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


    // fake discovery of the new walls
        unsigned char new_walls =  true_walls[maze.mouse_pos.x][maze.mouse_pos.y];
        for (size_t i = 0; i < 4; i++){
            if(WALLS[i]& new_walls){
                add_wall(&maze, static_cast<WALLS_DIR>(i));
            }
        }


    PATH_STACK path1;
    PATH_STACK path2;
    INNIT_STACK(path1);
    INNIT_STACK(path2);

    run_flood_fill(&path1, true_walls);
    //invert the maze
    maze.exit = start;
    maze.start = exit;
    cout<< "First run : \n";
    display_walls(&path1, &maze);

    run_flood_fill(&path2, true_walls);

    cout<< "Return path: \n";
    display_walls(&path2, &maze);
    return 0;
}