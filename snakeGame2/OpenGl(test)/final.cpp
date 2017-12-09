
#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <GL\glut.h>
#include <GL\GL.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stddef.h>
#include <iostream>
using namespace std;
int score = 0;

#define SNAKEHEADX vertex[0]
#define SNAKEHEADY vertex[1]
//#define BALLX vertex[4000]
//#define BALLY vertex[4001]
//#define BORDERX 0.5f
//#define BORDERY 0.5f
#define SNAKE_SPEED 100
#define BIGBALL_START 10002
long startTime = clock(); //return processor time consumed by the pre-code excution
enum DIRECTIONS {
	UP, DOWN, LEFT, RIGHT
};

int snakedirection = RIGHT;
//////////////////////////
void updateSnakeBody(int, float, float);
float ballX = -8; // -8 -> 8
float ballY = -8;
int snakelen = 1;
float xPos = 2.0f;
float yPos = 0.0f;
	//tail-->head			
vector <float> vertex;


void updateSnakeBody(int len, float xpos, float ypos);
void display();
void theCube();
void drawGrid();
void drawCar();
void reshape(int w, int h);
void loadObj(char *fname,GLuint listNum);
void init();
float cx = 0, cy = 0, cz = 0;
GLuint snakeHead;
GLuint snakeBody;
GLuint apple;

char ch = '1';
void keyboard(unsigned char key, int x, int y);




int main(int args , char **argv)
{
	vertex.reserve(20000);
	vertex.push_back(1);
	vertex.push_back(0);
	vertex.push_back(1.5);
	vertex.push_back(0);
	vertex.push_back(2);
	vertex.push_back(0);
	vertex.push_back(2.5);
	vertex.push_back(0);
	snakelen+=5;

	cout << vertex.size();
	glutInit(&args, argv);
   
	
		
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(800, 600);
		glutCreateWindow("");
		glutReshapeFunc(reshape);
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(display);
		snakeHead = glGenLists(1);
		snakeBody = glGenLists(2);
		apple = glGenLists(3);
		loadObj("data/mba1.obj",snakeHead);
		loadObj("data/apple2.obj", apple);
		//init();
		glutMainLoop();
	
	
	return 0;
}

void display()
{


	long endTime = clock();
	if (endTime - startTime >= SNAKE_SPEED)
	{
		switch (snakedirection)
		{
		case UP:
			yPos = yPos + 0.5;
			break;
		case DOWN:
			yPos = yPos - 0.5;
			break;
		case LEFT:
			xPos = xPos - 0.5;
			break;
		case RIGHT:
			xPos = xPos + 0.5;
			break;
		default:
			break;
		}
		startTime = clock();

		vertex.insert(vertex.end(), xPos);
		vertex.insert(vertex.end(), yPos);
		///////remove tail///////
		vertex.erase(vertex.begin());
		vertex.erase(vertex.begin());
	}
	//updateSnakeBody(snakelen, xPos, yPos);
	// ball eaten
	if (abs(xPos-ballX)<=.7  && abs(yPos-ballY)<=.7)
	{
		score++;
		//new apple
		float preAppleX = ballX;
		float preAppleY = ballY;
		while(ballX == preAppleX)
			ballX = -8 + (rand() % static_cast<int>(8 + 8 + 1));
		while(ballY == preAppleY)
			ballY = -8 + (rand() % static_cast<int>(8 + 8 + 1));
		 //incrase snake len
		 float newPosX,newPosY = yPos;

		 newPosX = xPos + 1;

		 vertex.push_back(newPosX);
		 vertex.push_back(newPosY);
	}
	

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(-13, 5, -25);
	glRotated(40, 1, 1, 0);
	gluLookAt(2.0, 7.0, 5.0, 0.0, 0.0, 0, 0, 3.0, -5.0);
	drawGrid();
	drawCar();
	
	//theCube();
	glutSwapBuffers();
}


void init()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35, 1.0f, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1, 0.1, 0.1, 1);

}

void drawGrid()
{
	float i;
	for (i = 0; i < 40; i+=0.5)
	{
		glPushMatrix();
		if (i < 20)
			glTranslatef(0, 0, i);
		if(i>=20)
		{
			glTranslatef(i - 20, 0, 0);
			glRotatef(-90, 0, 1, 0);
		}
		glBegin(GL_LINES);
		glColor3f(1, 1, 1);
		glLineWidth(1);
		glVertex3f(0, -0.1, 0);
		glVertex3f(19, -0.1, 0);
		glEnd();
		glPopMatrix();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 'w' && snakedirection!=UP)snakedirection = DOWN;
	if (key == 's'&& snakedirection != DOWN)snakedirection = UP;
	if (key == 'a'&& snakedirection != LEFT)snakedirection = LEFT;
	if (key == 'd'&& snakedirection != RIGHT)snakedirection = RIGHT;
	//f (key == 'q')cy -= 0.5;
	//if (key == 'z')cy += 0.5;

	glutPostRedisplay();
}


void loadObj(char *fname,GLuint listNum)
{
	FILE *fp;
	int read;
	GLfloat x, y, z;
	char ch;

	fp = fopen(fname, "r");
	if (!fp)
	{
		printf("can't open file %s\n", fname);
		return;
	}
	glPointSize(2.0);
	glNewList(listNum, GL_COMPILE);
	{
		glPushMatrix();
		glBegin(GL_POINTS);
		while (!(feof(fp)))
		{
			read = fscanf(fp, "%c %f %f %f", &ch, &x, &y, &z);
			if (read == 4 && ch == 'v')
			{
				glVertex3f(x, y, z);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glEndList();
	fclose(fp);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
	//glOrtho(-25,25,-2,2,0.1,100);	
	glMatrixMode(GL_MODELVIEW);
}


void drawCar()
{
	
	for (int  i = 0; i < vertex.size(); i+=2)
	{
		glPushMatrix();
		glTranslatef(10 + vertex.at(i), 0 , 10 + vertex.at(i+1));

		glColor3f(1.0, 0.23, 1.0);
		glScalef(0.1, 0.1, 0.1);

		glCallList(snakeHead);
		glPopMatrix();
	}
	
	glPushMatrix();
	glTranslatef(10+ballX, 0, 10+ballY);

	glColor3f(1, 0.0, 0);
	glScalef(0.02, 0.02, 0.02);
	glRotatef(190, 0, 0, 10);

	glCallList(apple);
	
	glPopMatrix();

}



