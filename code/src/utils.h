<<<<<<< HEAD

typedef enum {
    NO_ERROR,
    MOUSE_END,
    STACK_OVERFLOW,
    }RESULT;

typedef struct {
    int x;
    int y;
}COORDINATES;


#define EQUAL_COORD(coord_1, coord_2) (coord_1.x == coord_2.x && coord_1.y == coord_2.y)
#define ADD_COORD(coord, dX, dY) coord.x += dX; coord.y += dY;
#define MAX_UNSIGNED_BYTE 255

#define DEBBUG_PRINT(SENTENCE) \
    do{\
        if(DEBBUG){\
            SENTENCE;\
        }\
    }while(0)\
=======
void weighted_BFS(int x, int y, unsigned char currentDistance);
>>>>>>> refs/remotes/origin/main
