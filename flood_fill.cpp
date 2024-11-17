#include <cstring>
#include <stdint>

using namespace std;


#define MAZE_SIZE 8

#define LeftWall 1
#define UpWall 2
#define RightWall 4
#define DownWall 8


char matrix[MAZE_SIZE][MAZE_SIZE];
typedef struct {
    int x;
    int y;
}COORDINATES;



typedef struct{
    unsigned char matrix[MAZE_SIZE][MAZE_SIZE];
    unsigned char walls[MAZE_SIZE][MAZE_SIZE];
    COORDINATES mouse_pos;
    COORDINATES exit;
    COORDINATES start;
}Maze;

Maze maze;
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};
int WALLS[] = {LeftWall, UpWall, RightWall, DownWall};


void innit_maze(COORDINATES start, COORDINATES exit){
    maze.start = start;
    maze.exit = exit;
    maze.mouse_pos = start;
    std::memset(maze.walls, 0, MAZE_SIZE*MAZE_SIZE);
    std::memset(maze.matrix, 255, MAZE_SIZE*MAZE_SIZE);
}

void computeDistancesRecursive(int x, int y, int currentDistance) {
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
        if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE)
            continue;

        // Check if there's a wall blocking this direction
        if (maze.walls[x][y] & WALLS[dir])
            continue;

        // Recur into the next cell
        computeDistancesRecursive(nx, ny, currentDistance+1);
    }
}

char true_walls[MAZE_SIZE][MAZE_SIZE];


void display_matrix(){
    for (size_t i = 0; i < MAZE_SIZE; i++){
        for (size_t j = 0; j < MAZE_SIZE; j++){
            cout<< static_cast<int>(maze.matrix[i][j]) << " ";
        }
        cout<< "\n";
    }
}

void display_walls(){
  
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
            cout << "   "; // Space inside the cell
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
    innit_maze();
    
    true_walls = {
        {0b1011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

        {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},

        {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},

        {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},
         // Row 0
        {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},
         // Row 0
        {0b0011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100},
         // Row 0
        {0b1001, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b0110},
         // Row 0
        {0b1011, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1010, 0b1100}, // Row 0

    };

    memcpy(maze.walls, true_walls, MAZE_SIZE*MAZE_SIZE);
    
    display_matrix();
    return 0;
}
