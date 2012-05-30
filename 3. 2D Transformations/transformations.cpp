//Andre Brutus
#include <iostream>
#include <fstream>
#include <vector>
#include "GL/glut.h"

using namespace std;

#pragma region Global_Definitions 
//Frames per Animation
const double pi=3.14159265;
//Custom Objects
struct Point 
{
	float x;
	float y;
};
//central point
Point cp = {320.00,240.00};
//Globals
const int LENGTH=640;
const int WIDTH=480;
bool isRunning=false;
double t=0; // Parameter
vector<vector<Point>> actual;
int r_l=0,u_d=0, multiplier=1;
#pragma endregion Structures and Global Variables 

#pragma region Drawing_Functions
//Clears the screen
void clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

// Sets the Screensize and Background and foreground colors
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	gluOrtho2D(0.0,640.0,0.0,480.0);

}



// Draws a line between point a and point b
void line_draw(Point a, Point b)
{
	glBegin(GL_LINES);
	glVertex2i(a.x, a.y);
	glVertex2i(b.x ,b.y);
	glEnd();
	glColor3f(0.0f,0.0f,0.0f);
}
//draws lines in a poliline vector object
void drawpoli(vector<vector<Point>> polivec)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f,0.0f,0.0f);
	int nextloc=0;
	for(unsigned i=0; i<polivec.size(); i++)
		for(unsigned j=0; j<polivec[i].size()-1; j++)
			line_draw(polivec[i][j], polivec[i][j+1]);
}

//timed update
void update()
{
	drawpoli(actual);
	glFlush();
	return;
}

#pragma endregion OpenGL/glut drawing functions
#pragma region File_IO 
/*
* Loads 2D point vector 'actual'
*/

void read_file_data()
{
	ifstream ifs ( "dino.txt" , ifstream::in);
	float a,b;
	vector<int> v1;
	vector<int> v2;
	int n_poly=0;
	ifs >> a;
	n_poly=static_cast<int>(a);
	for(int i=0; i< n_poly; i++)
	{
		vector <Point> ha;
		ifs >> a;
		Point v;
		for(int h=0; h< a; h++)
		{
			ifs >> v.x;
			ifs >> v.y;
			ha.push_back(v);
		}
		actual.push_back(ha);
		ha.clear();
	}
	ifs.close();
}
#pragma endregion File I/O 
#pragma region Event_Handlers

// keyboard functions
void zoom(bool in)
{
	for(unsigned int scaling=0, scale=(glutGetModifiers() == GLUT_ACTIVE_SHIFT)? 10 : 1; scaling<scale; scaling++)
		for (unsigned i = 0;i<actual.size();i++)
			for (unsigned j = 0;j<actual[i].size();j++)
			{
				float tx,ty;
				if(in)
				{
					tx = 320.00*(-.01) + actual[i][j].x*1.01;
					ty = 240.00*(-.01) + actual[i][j].y*1.01;
				}
				else
				{
					tx = 320.00*(.01) + actual[i][j].x*.99;
					ty = 240.00*(.01) + actual[i][j].y*.99;
				}
				actual[i][j].x = tx;
				actual[i][j].y = ty;
			}
}
void rotate(bool a)
{

	float scale=(glutGetModifiers() == GLUT_ACTIVE_SHIFT)? 10 : 1;
	scale=a? scale: -scale;
	for(int i=0; i<actual.size(); i++)
		for(int j=0; j< actual[i].size(); j++)
		{			
			float tx = 320.00 + (actual[i][j].x-320.00)*cos(scale*3.14/180)- (actual[i][j].y-240.00)*sin(scale*3.14/180);
			float ty = 240.00 + (actual[i][j].x-320.00)*sin(scale*3.14/180)+ (actual[i][j].y-240.00)*cos(scale*3.14/180);
			actual[i][j].x = tx;
			actual[i][j].y = ty;
		}

}
void keyboard (unsigned char key, int x, int y) {
	//cout << key;
	int flip=1;
	multiplier=(glutGetModifiers() == GLUT_ACTIVE_SHIFT)? 10 : 1;
	switch(key)
	{
	case 'r':
	case 'R':
		rotate(1);
		break;
	case 'L':
	case 'l':
		rotate(0);
		break;
	case ',':
	case '<':
		zoom(0);
		break;
	case '.':
	case '>':
		zoom(1);
		break;
	}
}


void updateactual()
{
	if(r_l==u_d && u_d==0) return;
	for(unsigned i=0; i<actual.size(); i++)
	{
		for(unsigned j=0; j<actual[i].size(); j++)
		{
			actual[i][j].x+=r_l;
			actual[i][j].y+=u_d;
		}
	}
	r_l=u_d=0;
}

void keyboard_arrows (int key, int x, int y) {
	multiplier= (glutGetModifiers() == GLUT_ACTIVE_SHIFT)? 10 : 1;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		r_l-=multiplier;
		updateactual();
		break;
	case GLUT_KEY_RIGHT:
		r_l+=multiplier;
		updateactual();
		break;
	case GLUT_KEY_UP:
		u_d+=multiplier;
		updateactual();
		break;
	case GLUT_KEY_DOWN:
		u_d-=multiplier;
		updateactual();
		break;
	}
}

// Timed function, called (at least) every 10 ms
void Timed(int)
{
	update();
	glutTimerFunc(10, Timed, 5);
}
#pragma endregion Event (KeyUp, Mouse, TimerCallback etc) Handlers

int main(int argc,char **argv)
{
	// Initialize Vectors pts1, pts2
	read_file_data();

	glutInit(&argc,argv);
	glutInitWindowSize(LENGTH, WIDTH);
	glutInitDisplayMode (GLUT_SINGLE);
	glutCreateWindow("Tweening");
	clear();
	glutDisplayFunc(update);
	glutTimerFunc(0, Timed, 0);
	glutGetModifiers();
	glutKeyboardUpFunc(keyboard);
	glutSpecialUpFunc(keyboard_arrows);
	//glutMouseFunc(mouse);
	glClearColor(1.0,1.0,1.0,0.0);
	init();
	glutMainLoop();
}
