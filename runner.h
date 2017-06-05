#ifndef RUNNER_H
#define RUNNER_H

#include "cube.h"
class runner
{
public:
    runner();
    virtual ~runner();
    void loadGenes(double file[]);
    void solve(int moves);
    void makeMove(int i);
    static int getNumGenes();
    void setPuzzle(cube q);
    cube getCube();
    const static int NUM_GENES = 9*18*3;
    const static int TOP = 0, FRONT = 1, RIGHT = 2, BOTTOM = 3, BACK = 4, LEFT = 5;
    const static int X_AXIS = 0, Y_AXIS = 1, Z_AXIS = 2;
    int orientationToMove(int axis, int dir, int row);
    void chooseMove();
    double genes[NUM_GENES];
    double totalFitness;
    bool passedPreTest;

protected:

private:
    cube puzzle;
    int relativePos(int centerFace, int face, int row, int col);
    int currentSide = TOP;
    void relativeWeights(double probabilities[], int centerFace, int face, int row, int col);
};

#endif // RUNNER_H
