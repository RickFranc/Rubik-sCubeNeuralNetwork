#include "cube.h"
#include <GL/gl.h>
#include <stdio.h>
#include <GL/glut.h>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;



cube::cube()
{

}

cube::~cube()
{
    //dtor
}

bool cube::sideIsSolved(int side){
    int color = faces[side][1][1];
    for(int i = 0; i < 3; i++){
        for(int j =0; j < 3; j++){
            if(color != faces[side][i][j]){
                return false;
            }
        }
    }
    return true;
}

void cube::rotate(int dir, int row,int num)
{
    int temp[6][3][3];

    //copy
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            temp[TOP][i][j] = faces[TOP][i][j];
            temp[BOTTOM][i][j] = faces[BOTTOM][i][j];
            temp[FRONT][i][j] = faces[FRONT][i][j];
            temp[LEFT][i][j] = faces[LEFT][i][j];
            temp[BACK][i][j] = faces[BACK][i][j];
            temp[RIGHT][i][j] = faces[RIGHT][i][j];
        }
    }

    if(dir == X_AXIS) //top, right, bottom, left
    {
        for(int col = 0; col < 3; col++)
        {
            if(num == 1)
            {
                temp[TOP][col][row] = faces[LEFT][2-col][row];
                temp[RIGHT][col][row] = faces[TOP][col][row];
                temp[BOTTOM][2-col][row] = faces[RIGHT][col][row];
                temp[LEFT][2-col][row] = faces[BOTTOM][2-col][row];
            }
            else if(num == -1)
            {
                temp[TOP][col][row] = faces[RIGHT][col][row];
                temp[RIGHT][col][row] = faces[BOTTOM][2-col][row];
                temp[BOTTOM][2-col][row] = faces[LEFT][2-col][row];
                temp[LEFT][2-col][row] = faces[TOP][col][row];
            }
        }
    }
    else if(dir == Y_AXIS)  //front,left, back,right
    {

        for(int col = 0; col < 3; col++)
        {
            if(num == 1)
            {
                temp[FRONT][col][row] = faces[RIGHT][row][col];
                temp[LEFT][row][2-col] = faces[FRONT][col][row];
                temp[BACK][2-col][row] = faces[LEFT][row][2-col];
                temp[RIGHT][row][col] = faces[BACK][2-col][row];
            }
            else if(num == -1)
            {
                temp[FRONT][col][row] = faces[LEFT][row][2-col];
                temp[LEFT][row][2-col] = faces[BACK][2-col][row];
                temp[BACK][2-col][row] = faces[RIGHT][row][col];
                temp[RIGHT][row][col] = faces[FRONT][col][row];
            }
        }


    }
    else if(dir == Z_AXIS)  //front, top, back, bottom
    {
        for(int col = 0; col < 3; col++)
        {
            if(num == 1)
            {
                temp[FRONT][2-row][col] = faces[BOTTOM][2-row][col];
                temp[TOP][2-row][2-col] = faces[FRONT][2-row][col];
                temp[BACK][2-row][2-col] = faces[TOP][2-row][2-col];
                temp[BOTTOM][2-row][col] = faces[BACK][2-row][2-col];
            }
            else if(num == -1)
            {
                temp[FRONT][2-row][col] = faces[TOP][2-row][2-col];
                temp[TOP][2-row][2-col] = faces[BACK][2-row][2-col];
                temp[BACK][2-row][2-col] = faces[BOTTOM][2-row][col];
                temp[BOTTOM][2-row][col] = faces[FRONT][2-row][col];

            }
        }
    }

    if(row != 1)
        rotateSides(faces,temp,dir,row,num);

    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                faces[i][j][k] = temp[i][j][k];
            }
        }
    }
}

void cube::rotateSides(int faces[][3][3],int temp[][3][3], int dir, int row, int num)
{
    int side = TOP;
    if(dir == X_AXIS) //top, right, bottom, left
    {
        if(row == 2)
            side = BACK;
        else
            side = FRONT;
    }
    else if(dir == Y_AXIS)  //front,left, back,right
    {
        if(row == 0)
            side = TOP;
        else
            side = BOTTOM;
        num *= -1;
    }
    else if(dir == Z_AXIS)  //front, top, back, bottom
    {
        if(row == 2)
            side = LEFT;
        else
            side = RIGHT;
        num *= -1;
    }
    if(num == -1)
    {
        temp[side][0][0] = faces[side][2][0];
        temp[side][1][0] = faces[side][2][1];
        temp[side][2][0] = faces[side][2][2];
        temp[side][2][1] = faces[side][1][2];
        temp[side][2][2] = faces[side][0][2];
        temp[side][1][2] = faces[side][0][1];
        temp[side][0][2] = faces[side][0][0];
        temp[side][0][1] = faces[side][1][0];
    }
    else if(num == 1)
    {
        temp[side][0][0] = faces[side][0][2];
        temp[side][1][0] = faces[side][0][1];
        temp[side][2][0] = faces[side][0][0];
        temp[side][2][1] = faces[side][1][0];
        temp[side][2][2] = faces[side][2][0];
        temp[side][1][2] = faces[side][2][1];
        temp[side][0][2] = faces[side][2][2];
        temp[side][0][1] = faces[side][1][2];
    }
}

int cube::score()
{
    int score = 0;

    for(int f = 0; f < 6; f++)
    {
        int colorCount[6];
        for(int i = 0; i < 6; i++)
            colorCount[i]=0;

        for(int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                int color = faces[f][i][j];
                colorCount[color]++;
            }
        }

        int numColors = -1;
        for(int i = 0; i < 6; i++)
        {
            if(colorCount[i]>0)
                numColors++;

        }
        score += numColors*numColors;
    }
    return score;
}


void cube::mix(int level)
{
    for(int i = 0; i < level; i++) //number of random rotations
    {
        int dir = rand()%3;
        int row = rand()%3;
        int num = rand()%2;

        if(num == 0)
            num = -1;
        rotate(dir,row,num);
        if(level == 1)
            rotate(dir,row,num);
    }
}

void cube::initialize()
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                faces[i][j][k] = i;
            }
        }
    }
}

void cube::display(void)
{
    glClear( GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if(rot)
    {
        const double t = glutGet(GLUT_ELAPSED_TIME);
        glRotated(ANGLE*sin(t/1000),1,0,0);
        glRotated(ANGLE*cos(t/777 + 123),0,1,0);
        glRotated(ANGLE,0,0,1);
    }

    glColor3f(0,0,0);
    glBegin((GL_QUADS));
    glVertex3f(0.1, 0.1, 0.1);//front
    glVertex3f(0.1, 9.9, 0.1);
    glVertex3f(9.9, 9.9, 0.1);
    glVertex3f(9.9, 0.1, 0.1);

    glVertex3f(0.1, 0.1, 9.9);//back
    glVertex3f(0.1, 9.9, 9.9);
    glVertex3f(9.9, 9.9, 9.9);
    glVertex3f(9.9, 0.1, 9.9);

    glVertex3f(0.1, 0.1, 0.1);//top
    glVertex3f(0.1, 0.1, 9.9);
    glVertex3f(9.9, 0.1, 9.9);
    glVertex3f(9.9, 0.1, 0.1);

    glVertex3f(0.1, 9.9, 0.1);//bottom
    glVertex3f(0.1, 9.9, 9.9);
    glVertex3f(9.9, 9.9, 9.9);
    glVertex3f(9.9, 9.9, 0.1);

    glVertex3f(0.1, 0.1, 0.1);//left
    glVertex3f(0.1, 9.9, 0.1);
    glVertex3f(0.1, 9.9, 9.9);
    glVertex3f(0.1, 0.1, 9.9);

    glVertex3f(9.9, 0.1, 0.1);//right
    glVertex3f(9.9, 9.9, 0.1);
    glVertex3f(9.9, 9.9, 9.9);
    glVertex3f(9.9, 0.1, 9.9);
    glEnd();

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            glBegin(GL_QUADS);
            setColor(faces[FRONT][i][j]);
            glVertex3f(1+i*3, 1+j*3, 0);
            glVertex3f(3+i*3, 1+j*3, 0);
            glVertex3f(3+i*3, 3+j*3, 0);
            glVertex3f(1+i*3, 3+j*3, 0);
            setColor(faces[BACK][i][j]);
            glVertex3f(1+i*3, 1+j*3, 10);
            glVertex3f(3+i*3, 1+j*3, 10);
            glVertex3f(3+i*3, 3+j*3, 10);
            glVertex3f(1+i*3, 3+j*3, 10);
            setColor(faces[TOP][i][j]);
            glVertex3f(1+i*3,0, 1+j*3);
            glVertex3f(3+i*3,0, 1+j*3);
            glVertex3f(3+i*3,0, 3+j*3);
            glVertex3f(1+i*3,0, 3+j*3);
            setColor(faces[BOTTOM][i][j]);
            glVertex3f(1+i*3,10, 1+j*3);
            glVertex3f(3+i*3,10, 1+j*3);
            glVertex3f(3+i*3,10, 3+j*3);
            glVertex3f(1+i*3,10, 3+j*3);
            setColor(faces[LEFT][i][j]);
            glVertex3f(0,1+i*3, 1+j*3);
            glVertex3f(0,3+i*3, 1+j*3);
            glVertex3f(0,3+i*3, 3+j*3);
            glVertex3f(0,1+i*3, 3+j*3);
            setColor(faces[RIGHT][i][j]);
            glVertex3f(10,1+i*3, 1+j*3);
            glVertex3f(10,3+i*3, 1+j*3);
            glVertex3f(10,3+i*3, 3+j*3);
            glVertex3f(10,1+i*3, 3+j*3);


            glEnd();
        }
    }
    glFlush();
}

void cube::setColor(int color)
{
    if(color == YELLOW)
        glColor3f(1.0, 1.0, 0.0);
    else if(color == RED)
        glColor3f(1.0, 0.0, 0.0);
    else if(color == BLUE)
        glColor3f(0.0, 0.0, 1.0);
    else if(color == WHITE)
        glColor3f(1.0, 1.0, 1.0);
    else if(color == ORANGE)
        glColor3f(1.0, 0.5, 0.0);
    else if(color == GREEN)
        glColor3f(0.0, 1.0, 0.0);
}

void cube::printCube()
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                int color = faces[i][k][j];
                if(color == YELLOW)
                    cout << "Y ";
                else if(color == RED)
                    cout << "R ";
                else if(color == BLUE)
                    cout << "B ";
                else if(color == WHITE)
                    cout << "W ";
                else if(color == ORANGE)
                    cout << "O ";
                else if(color == GREEN)
                    cout << "G ";
            }
            cout << endl;
        }
        cout << endl;
    }
}


cube cube::getCopy()
{
    cube newCube;
    newCube.setCube(faces);
    return newCube;
}

void cube::setCube(int newFace[][3][3])
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                faces[i][j][k] = newFace[i][j][k];
            }
        }
    }
}


int cube::getColor(int face, int row, int col)
{
    return faces[face][row][col];
}

bool cube::isSolved()
{
    for(int i = 0; i < 6; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                if(faces[i][0][0] != faces[i][j][k])
                    return false;
            }
        }
    }
    return true;
}
