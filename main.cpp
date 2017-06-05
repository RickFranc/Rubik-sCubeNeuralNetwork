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
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include <runner.h>
#include <cube.h>

using namespace std;

void drawLoop();
void idle();
void keyPress(unsigned char key, int x, int y);
void fileLoad();
void setupWeights();
void display();
int runTest();
void mate(int parents, int level);
cube c, displayCube;
void runOnce(int level);
void preTest(int level);
const int POP = 10;
int ROUNDS_PER_GENERATION = 100,GENERATIONS = 100000;
const double MAX_MUTATION = 0.2, CHANCE_RANDOM =3;
runner r[POP];
int level = 1;

ifstream in_stream;
ofstream out_stream;

int main(int argc, char **argv)
{
    srand(time(0));

    cout << "Enter the number of generations:" << endl;
    cin >> GENERATIONS;
    cout << "Enter the number of rounds per generation(100 recommended):"<<endl;
    cin >>ROUNDS_PER_GENERATION;

    fileLoad();

    int parents;
    for(int i = 0; i < GENERATIONS; i++)
    {

        cout << "Generation #"<<i<< " Level #" << level<<endl;
        parents = runTest();
        mate(parents, level);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(800,100);
    glutInitWindowSize(300,300);
    glutCreateWindow ("square");

    glClearColor(1.0, 1.0, 1.0, 0.0);         // black background
    glMatrixMode(GL_MODELVIEW);              // setup viewing projection
    glLoadIdentity();                           // start with identity matrix
    glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);   // setup a 10x10x2 viewing world

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyPress);

    glTranslated(-6.5,-6.5,-6.5);
    glutMainLoop();


    return EXIT_SUCCESS;
}

void idle(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME);

    if((int)(t) % 50 < 1)
        glutPostRedisplay();
}

void display(void)
{
    displayCube.display();
}

int xxx = 0;
void keyPress(unsigned char key, int x, int y)
{
    if(key == 'q')
        displayCube.rotate(0,0,1);
    else if(key == 'w')
        displayCube.rotate(0,1,1);
    else if(key == 'e')
        displayCube.rotate(0,2,1);
    else if(key == 'a')
        displayCube.rotate(1,0,1);
    else if(key == 's')
        displayCube.rotate(1,1,1);
    else if(key == 'd')
        displayCube.rotate(1,2,1);
    else if(key == 'z')
        displayCube.rotate(2,0,1);
    else if(key == 'x')
        displayCube.rotate(2,1,1);
    else if(key == 'c')
        displayCube.rotate(2,2,1);
    else if(key == '`')
        displayCube = c;
    else if(key == 'm')//Mix cube
    {
        c.mix(2);
        displayCube = c;
    }
    else if(key == 'i')//Initialize cube
    {
        c.initialize();
        displayCube = c;
    }
    else if(key == 'n')//Display score
        cout<< "Cube score: " <<displayCube.score()<<endl;
    else if(isdigit(key))
    {
        int num = (int)key-48;
        displayCube = r[num].getCube();
    }
    else if(key == 'r')//stop/start rotating
    {
        displayCube.rot = !displayCube.rot;
    }
    else if(key == 'y')//Load display cube to best runner
    {
        r[0].setPuzzle(displayCube);
        displayCube = r[0].getCube();
    }
    else if(key == 'u')//make one move using best runner
    {
        if(!r[0].getCube().isSolved())
        {
            r[0].chooseMove();
            displayCube = r[0].getCube();
        }
    }
}

void fileLoad()
{
    setupWeights();
    in_stream.open("weights.txt");
    string line;
    in_stream >> line;
    int setNum = 0, spot = 0;
    double file[r[0].getNumGenes()];
    while(line != "")
    {
        if(line == ";")
        {
            r[setNum].loadGenes(file);
            spot = 0;
            setNum++;
        }
        else
        {

            file[spot] = atof(line.c_str());
            spot++;
        }
        line = "";
        in_stream >> line;
    }
    in_stream.close();



}

void setupWeights()
{
    srand(time(0));
    out_stream.open("weights.txt");
    for(int i = 0; i < POP; i++)
    {
        for(int j = 0; j < r[0].getNumGenes(); j++)
        {
            out_stream << 2.0*((double) rand()/RAND_MAX)-1;
            out_stream << " ";
        }
        out_stream << ";\n";
    }
    out_stream.close();
}


int runTest()
{
    preTest(level);
    for(int j = 0; j < ROUNDS_PER_GENERATION; j++)
    {
        runOnce(level);
    }
    double highestFitness= -99999999, highestFitness2 = -9999999;
    int parent1 = 0, parent2 = 1;
    for(int i = 0; i < POP; i++)
    {
        if(r[i].passedPreTest)
        {
            double fitness = 100-r[i].totalFitness/ROUNDS_PER_GENERATION;
            if(fitness > highestFitness2)
            {
                if(fitness > highestFitness)
                {
                    highestFitness2 = highestFitness;
                    parent2 = parent1;
                    highestFitness = fitness;
                    parent1 = i;
                }
                else
                {
                    highestFitness2 = fitness;
                    parent2 = i;
                }
            }
        }
    }
    if(highestFitness >= 0)
    {
        cout << "Runner " << parent1 << " has the highest fitness of " << highestFitness << endl;
        if(highestFitness2 >= 0)
        {
            cout << "Runner " << parent2 << " has the second highest fitness of " << highestFitness2 << endl;
        }
        else
        {
            cout << "No second runner passed the pretest" << endl;
        }
    }
    else
    {
        cout << "No runner passed the pretest" << endl;
    }
    if(highestFitness >= 100 && highestFitness2 >= 100)
        level++;
    return parent1+POP*parent2;
}

void runOnce(int level)
{
    c.initialize();
    while(c.isSolved())
    {
        c.mix(level);
    }
    for(int i = 0; i < POP; i++)
    {
        if(r[i].passedPreTest)
        {
            r[i].setPuzzle(c);
            r[i].solve(level);
        }
    }
}

void mate(int parents, int level)
{
    //setup
    runner nextGen[POP];

    runner parent1 = r[parents%POP];
    int loc2 = (parents - parents%POP)/POP;
    runner parent2 = r[loc2];
    int numGenes = parent1.getNumGenes();
    double bestGenes[numGenes],secondBestGenes[numGenes];
    for(int i = 0; i < numGenes; i++)
    {
        bestGenes[i] = parent1.genes[i];
        secondBestGenes[i] = parent2.genes[i];
    }

    //keep best
    nextGen[0].loadGenes(bestGenes);
    //and second best
    nextGen[1].loadGenes(secondBestGenes);
    //mix and mutate
    for(int i = 2; i < POP -1; i++)
    {
        double newGenes[numGenes];
        for(int i = 0; i < numGenes; i++)
        {
            int random = rand()%50*level;
            if(random < CHANCE_RANDOM)//Occasional completely random gene
            {
                newGenes[i] = 2.0*((double) rand()/RAND_MAX)-1;
            }
            else//choose a random value between best and second best and add a small random factor
            {
                double value = bestGenes[i]+(secondBestGenes[i]-bestGenes[i])*((double) rand()/RAND_MAX) + MAX_MUTATION*(2/(1+level)) * 2*(((double) rand()/RAND_MAX)-0.5);
                if(value > 1)
                    value = 1;
                else if(value < -1)
                    value = -1;
                newGenes[i] = value;
            }
        }
        nextGen[i].loadGenes(newGenes);
    }

    //last is random
    double newGenes[numGenes];
    for(int i = 0; i < numGenes; i++)
    {
        newGenes[i] = 2.0*((double) rand()/RAND_MAX)-1;
    }
    nextGen[POP-1].loadGenes(newGenes);


    //load nextGen
    for(int i = 0; i < POP; i++)
        r[i]= nextGen[i];
}

void preTest(int level)
{
    for(int i = 0; i < POP; i++)
    {
        r[i].passedPreTest = true;
    }

    for(int diff = 1; diff < level; diff++)
    {
        for(int i = 0; i < ROUNDS_PER_GENERATION; i++)
        {
            c.initialize();
            while(c.isSolved())
            {
                c.mix(diff);
            }
            for(int i = 0; i < POP; i++)
            {
                if(r[i].passedPreTest)
                {
                    r[i].totalFitness = 0;
                    r[i].setPuzzle(c);
                    r[i].solve(diff);
                    if(r[i].totalFitness != 0)
                        r[i].passedPreTest = false;
                }
            }
        }
    }
}

