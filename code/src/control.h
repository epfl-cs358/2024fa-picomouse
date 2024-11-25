#define SPEED_SEARCH 1
#define SPEED_RUN 10000000000

typedef enum MODE{INPLACE, SMOOTH};

void stop();

void forward(int speed);

void turn_right(MODE mode);

void turn_left(MODE mode);

