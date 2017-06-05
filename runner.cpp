#include "runner.h"
#include "cube.h"
#include <iostream>
#include <cstdlib>


using namespace std;

runner::runner()
{
    totalFitness = 0;
}

runner::~runner()
{
    //dtor
}

void runner::setPuzzle(cube q)
{
    puzzle = q;
}

void runner::loadGenes(double file[])
{
    for(int i = 0; i < NUM_GENES; i++)
    {
        genes[i] = file[i];
    }
}


void runner::solve(int moves)
{
    if(moves == 1)
        moves++; //1st level is just double of the same moves

    for(int i = 0; i < moves; i++)
    {
        chooseMove();
        if(puzzle.isSolved())
        {
            totalFitness += 0;//i;
            return;
        }
    }
    if(puzzle.isSolved())
    {
        totalFitness += 0;//i;
        return;
    }
    totalFitness += 100;//NUM_MOVES + puzzle.score()+100;
}

void runner::chooseMove()
{
    //initialize probabilities
    double possMoves[18], deepNodes[18];
    for(int i = 0; i < 18; i++)
        possMoves[i]=deepNodes[i]=0;

    int colorCenters[6];
    for(int face = 0; face < 6; face++)
        colorCenters[puzzle.getColor(face,1,1)]=face;

    for(int face = 0; face < 6; face++)
    {
        for(int row = 0; row < 3; row++)
        {
            for(int col = 0; col < 3; col++)
            {
                if(row != 1 || col != 1)
                {
                    //find relative position, add weights
                    relativeWeights(deepNodes,colorCenters[puzzle.getColor(face,row,col)],face,row,col);
                }
            }
        }
    }

    //deep nodes to outputs
    //for(int i = 0; i < 18; i++){
    //    for(int j = 0; j < 18; j++){
    //         possMoves[j] += deepNodes[i]*genes[9*18+i*18+j];
    //    }
    //}
    //choose move based on probabilities

    double maxProb = -999999;
    int bestMove = 0;
    for(int i = 0; i < 18; i++)
    {
        if(deepNodes[i]> maxProb)
        {
            bestMove = i;
            maxProb = deepNodes[i];
        }
    }
    makeMove(bestMove);
}

void runner::makeMove(int i)
{
    int axis;
    int dir =1;
    int row;
    if(i >= 9)
    {
        dir = -1;
        i-=9;
    }
    row = i%3;
    i -= row;
    axis = i/3;
    puzzle.rotate(axis,row,dir);
}

int runner::getNumGenes()
{
    return NUM_GENES;
}

cube runner::getCube()
{
    return puzzle;
}

int runner::relativePos(int centerFace, int face, int row, int col)
{
    bool corner = (row != 1 && col != 1);

    if(face == centerFace)
    {
        if(corner)
            return 2;
        else
            return 1;
    }
    else if(face == centerFace + 3 || centerFace == face + 3)  //opposite sides
    {
        if(corner)
            return 9;
        else
            return 8;
    }
    else
    {
        if(centerFace == puzzle.FRONT)
        {
            if(col == 2)
            {
                if(corner)
                {
                    return 6;
                }
                else
                {
                    return 7;
                }
            }
            else if(col == 1)
            {
                return 5;
            }
            else
            {
                if(corner)
                {
                    return 4;
                }
                else
                {
                    return 3;
                }
            }
        }
        else if(centerFace == puzzle.BACK)
        {
            if(col == 0)
            {
                if(corner)
                {
                    return 6;
                }
                else
                {
                    return 7;
                }
            }
            else if(col == 1)
            {
                return 5;
            }
            else
            {
                if(corner)
                {
                    return 4;
                }
                else
                {
                    return 3;
                }
            }
        }
        else if(centerFace == puzzle.LEFT)
        {
            if(row == 2)
            {
                if(corner)
                {
                    return 6;
                }
                else
                {
                    return 7;
                }
            }
            else if(row == 1)
            {
                return 5;
            }
            else
            {
                if(corner)
                {
                    return 4;
                }
                else
                {
                    return 3;
                }
            }
        }
        else if(centerFace == puzzle.RIGHT)
        {
            if(row == 0)
            {
                if(corner)
                {
                    return 6;
                }
                else
                {
                    return 7;
                }
            }
            else if(row == 1)
            {
                return 5;
            }
            else
            {
                if(corner)
                {
                    return 4;
                }
                else
                {
                    return 3;
                }
            }
        }
        else if(centerFace == puzzle.TOP)
        {
            if(face == puzzle.FRONT || face == puzzle.BACK)
            {
                if(col == 2)
                {
                    if(corner)
                    {
                        return 6;
                    }
                    else
                    {
                        return 7;
                    }
                }
                else if(col == 1)
                {
                    return 5;
                }
                else
                {
                    if(corner)
                    {
                        return 4;
                    }
                    else
                    {
                        return 3;
                    }
                }
            }
            else
            {
                if(row == 2)
                {
                    if(corner)
                    {
                        return 6;
                    }
                    else
                    {
                        return 7;
                    }
                }
                else if(row == 1)
                {
                    return 5;
                }
                else
                {
                    if(corner)
                    {
                        return 4;
                    }
                    else
                    {
                        return 3;
                    }
                }

            }
        }
        else if(centerFace == puzzle.BOTTOM)
        {
            if(face == puzzle.FRONT || face == puzzle.BACK)
            {
                if(col == 0)
                {
                    if(corner)
                    {
                        return 6;
                    }
                    else
                    {
                        return 7;
                    }
                }
                else if(col == 1)
                {
                    return 5;
                }
                else
                {
                    if(corner)
                    {
                        return 4;
                    }
                    else
                    {
                        return 3;
                    }
                }
            }
            else
            {
                if(row == 0)
                {
                    if(corner)
                    {
                        return 6;
                    }
                    else
                    {
                        return 7;
                    }
                }
                else if(row == 1)
                {
                    return 5;
                }
                else
                {
                    if(corner)
                    {
                        return 4;
                    }
                    else
                    {
                        return 3;
                    }
                }

            }
        }
    }
}

void runner::relativeWeights(double nodes[], int centerFace, int face, int row, int col)
{
    bool corner = (row != 1 && col != 1);
    int relPos = relativePos(centerFace,face,row,col);
    int spot = row*3 +col+1;
    int dir = 1;
    int side = 1;
    int axis;
    int crossFaceAxis, crossCenterAxis,crossFaceSide, crossCenterSide,crossFaceDir =1,crossCenterDir =1;

    if(centerFace == FRONT)
    {
        if(face == FRONT || face == BACK)
        {
            if(spot == 2 || spot == 3)
            {
                face = TOP;
            }
            else if(spot == 6 || spot == 9)
            {
                face = RIGHT;
            }
            else if(spot == 7 || spot == 8)
            {
                face = BOTTOM;
            }
            else if(spot == 1 || spot == 4)
            {
                face = LEFT;
            }
        }
        if(face == TOP)
        {
            axis = Z_AXIS;
            if(row == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
        else if(face == BOTTOM)
        {
            axis = Z_AXIS;
            dir = -1;
            if(row == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
        else if(face == LEFT)
        {
            axis = Y_AXIS;
            if(row == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
        else if(face == RIGHT)
        {
            axis = Y_AXIS;
            dir = -1;
            if(row == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
    }
    else if(centerFace == BACK)
    {
        if(face == FRONT || face == BACK)
        {
            if(spot == 2 || spot == 1)
            {
                face = TOP;
            }
            else if(spot == 4 || spot == 7)
            {
                face = LEFT;
            }
            else if(spot == 9 || spot == 8)
            {
                face = BOTTOM;
            }
            else if(spot == 6 || spot == 3)
            {
                face = RIGHT;
            }
        }
        if(face == TOP)
        {
            axis = Z_AXIS;
            dir = -1;
            if(row == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
        else if(face == BOTTOM)
        {
            axis = Z_AXIS;
            if(row == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
        else if(face == LEFT)
        {
            axis = Y_AXIS;
            dir = -1;
            if(row == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
        else if(face == RIGHT)
        {
            axis = Y_AXIS;
            if(row == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
    }
    else if(centerFace == TOP)
    {
        if(face == TOP || face == BOTTOM)
        {
            if(spot == 2 || spot == 1)
            {
                face = FRONT;
            }
            else if(spot == 6 || spot == 3)
            {
                face = RIGHT;
            }
            else if(spot == 9 || spot == 8)
            {
                face = BACK;
            }
            else if(spot == 7 || spot == 4)
            {
                face = LEFT;
            }
        }
        if(face == FRONT)
        {
            axis = Z_AXIS;
            dir = -1;
            if(row == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
        else if(face == BACK)
        {
            axis = Z_AXIS;
            if(row == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
        else if(face == LEFT)
        {
            axis = X_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
        else if(face == RIGHT)
        {
            axis = X_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
    }
    else if(centerFace == BOTTOM)
    {
        if(face == TOP || face == BOTTOM)
        {
            if(spot == 2 || spot == 3)
            {
                face = FRONT;
            }
            else if(spot == 6 || spot == 9)
            {
                face = RIGHT;
            }
            else if(spot == 7 || spot == 8)
            {
                face = BACK;
            }
            else if(spot == 1 || spot == 4)
            {
                face = LEFT;
            }
        }
        if(face == FRONT)
        {
            axis = Z_AXIS;
            if(row == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
        else if(face == BACK)
        {
            axis = Z_AXIS;
            dir = -1;
            if(row == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
        else if(face == LEFT)
        {
            axis = X_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
        else if(face == RIGHT)
        {
            axis = X_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
    }
    else if(centerFace == LEFT)
    {
        if(face == LEFT || face == RIGHT)
        {
            if(spot == 2 || spot == 1)
            {
                face = TOP;
            }
            else if(spot == 6 || spot == 3)
            {
                face = BACK;
            }
            else if(spot == 9 || spot == 8)
            {
                face = BOTTOM;
            }
            else if(spot == 7 || spot == 4)
            {
                face = LEFT;
            }
        }
        if(face == TOP)
        {
            axis = X_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
        else if(face == BOTTOM)
        {
            axis = X_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
        else if(face == FRONT)
        {
            axis = Y_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = -1;
            crossFaceDir = -1;
            crossCenterDir = 1;
        }
        else if(face == BACK)
        {
            axis = Y_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = -1;
            crossCenterSide = 1;
            crossFaceDir = -1;
            crossCenterDir = -1;
        }
    }
    else if(centerFace == RIGHT)
    {
        if(face == LEFT || face == RIGHT)
        {
            if(spot == 2 || spot == 3)
            {
                face = TOP;
            }
            else if(spot == 6 || spot == 9)
            {
                face = BACK;
            }
            else if(spot == 7 || spot == 8)
            {
                face = BOTTOM;
            }
            else if(spot == 1 || spot == 4)
            {
                face = FRONT;
            }
        }
        if(face == TOP)
        {
            axis = X_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
        else if(face == BOTTOM)
        {
            axis = X_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
        else if(face == FRONT)
        {
            axis = Y_AXIS;
            if(col == 2)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = -1;
            crossFaceDir = 1;
            crossCenterDir = 1;
        }
        else if(face == BACK)
        {
            axis = Y_AXIS;
            dir = -1;
            if(col == 0)
                side = -1;
            crossFaceSide = 1;
            crossCenterSide = 1;
            crossFaceDir = 1;
            crossCenterDir = -1;
        }
    }


    if(dir == -1)
        side = -side;
    int move;
    //to face
    move = orientationToMove(axis, dir,side+1);
    nodes[move] += genes[relPos*18];
    move = orientationToMove(axis, dir,1);
    nodes[move] += genes[relPos*18+1];
    move = orientationToMove(axis, dir,1-side);
    nodes[move] += genes[relPos*18+2];
    //away from face
    move = orientationToMove(axis, -dir,1-side);
    nodes[move] += genes[relPos*18+9];
    move = orientationToMove(axis, -dir,1);
    nodes[move] += genes[relPos*18+10];
    move = orientationToMove(axis, -dir,1+side);
    nodes[move] += genes[relPos*18+11];


    if(axis == X_AXIS)
    {
        if(face == TOP || face == BOTTOM)
        {
            crossFaceAxis = Z_AXIS;
            crossCenterAxis = Y_AXIS;
        }
        else
        {
            crossFaceAxis = Y_AXIS;
            crossCenterAxis = Z_AXIS;
        }
    }
    else if(axis == Y_AXIS)
    {
        if(face == FRONT || face == BACK)
        {
            crossFaceAxis = Z_AXIS;
            crossCenterAxis = X_AXIS;
        }
        else
        {
            crossFaceAxis = X_AXIS;
            crossCenterAxis = Z_AXIS;
        }
    }
    else if(axis == Z_AXIS)
    {
        if(face == TOP || face == BOTTOM)
        {
            crossFaceAxis = X_AXIS;
            crossCenterAxis = Y_AXIS;
        }
        else
        {
            crossFaceAxis = Y_AXIS;
            crossCenterAxis = X_AXIS;
        }
    }

    if(side == -1)
    {
        crossFaceDir = -crossFaceDir;
        crossCenterDir = -crossCenterDir;
    }

//across face
    move = orientationToMove(crossFaceAxis, crossFaceDir,crossFaceSide+1);
    nodes[move] += genes[relPos*18+3];
    move = orientationToMove(crossFaceAxis, crossFaceDir,1);
    nodes[move] += genes[relPos*18+4];
    move = orientationToMove(crossFaceAxis, crossFaceDir,1-crossFaceSide);
    nodes[move] += genes[relPos*18+5];
    //reverse face
    move = orientationToMove(crossFaceAxis, -crossFaceDir,1-crossFaceSide);
    nodes[move] += genes[relPos*18+12];
    move = orientationToMove(crossFaceAxis, -crossFaceDir,1);
    nodes[move] += genes[relPos*18+13];
    move = orientationToMove(crossFaceAxis, -crossFaceDir,1+crossFaceSide);
    nodes[move] += genes[relPos*18+14];
    // across center
    move = orientationToMove(crossCenterAxis, crossCenterDir,crossCenterSide+1);
    nodes[move] += genes[relPos*18+6];
    move = orientationToMove(crossCenterAxis, crossCenterDir,1);
    nodes[move] += genes[relPos*18+7];
    move = orientationToMove(crossCenterAxis, crossCenterDir,1-crossCenterSide);
    nodes[move] += genes[relPos*18+8];
    //reverse center
    move = orientationToMove(crossCenterAxis, crossCenterDir,1-crossCenterSide);
    nodes[move] += genes[relPos*18+15];
    move = orientationToMove(crossCenterAxis, crossCenterDir,1);
    nodes[move] += genes[relPos*18+16];
    move = orientationToMove(crossCenterAxis, crossCenterDir,1+crossCenterSide);
    nodes[move] += genes[relPos*18+17]; //

}

int runner::orientationToMove(int axis, int dir, int row)
{
    int move = axis*3+row;
    if(dir == -1)
        move += 9;
    return move;
}
