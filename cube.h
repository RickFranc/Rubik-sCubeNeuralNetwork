#ifndef CUBE_H
#define CUBE_H


class cube
{
public:
    cube();
    virtual ~cube();
    void initialize();
    void mix(int level);
    void rotate(int dir, int row,int num);
    void printCube();
    int score();
    void display();
    cube getCopy();
    int getColor(int face, int row, int col);
    bool isSolved();
    bool sideIsSolved(int side);

    const static int TOP = 0, FRONT = 1, RIGHT = 2, BOTTOM = 3, BACK = 4, LEFT = 5;
    const static int YELLOW = 0, RED = 1, BLUE = 2, WHITE = 3, ORANGE = 4, GREEN = 5;
    const static int X_AXIS = 0, Y_AXIS = 1, Z_AXIS = 2;
    const static int X_LOC = 0, Y_LOC= 0, Z_LOC = 0;

    bool rot = true;
    double ANGLE = 0.5;

protected:

private:
    void setColor(int color);
    void rotateSides(int faces[][3][3],int temp[][3][3],  int dir,int row, int num);
    void setCube(int newFace[][3][3]);
    int faces[6][3][3];
};

#endif // CUBE_H
