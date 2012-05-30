//Andre Brutus
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glut.h>

#include <math.h>
using namespace std;

#pragma region Global_Definitions 
//Custom Objects
struct Point 
{
	float x;
	float y;
};

struct Line_eq
{
	int y,m,x,b,change_y, change_x;
};

//Globals
const int LENGTH=480;
const int WIDTH=360;

vector<Point> pts1;
vector<Point> pts2;

#pragma region Drawing_Functions
//Clears the screen
void clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

// Sets the Screensize and Background and foreground colors
void init()
{
	glClearColor(1.0,1.0,1.0,0.0);
	glColor3f(0.0f,0.0f,0.0f);
	gluOrtho2D(0.0,640.0,0.0,480.0);

}

// Draws a point on the screen
void drawpoint(Point a)
{
	glVertex2i(a.x,WIDTH-a.y);
}



// Draws a line between point a and point b
void line_draw(Point a, Point b)
{
	glBegin(GL_LINES);
	glVertex2i(a.x, a.y);
	glVertex2i(b.x ,b.y);
	glEnd();
	glFlush();
	//cout << "Drawing line from " << p2s(a) << "to " << p2s(b) << endl;
	glColor3f(0.0f,0.0f,0.0f);
}

void drawhouse(vector<Point> pt)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f,0.0f,0.0f);
	int nextloc=0;
	for(int i=0; i<pts1.size()-1 && pts1.size(); i++)
	{
		nextloc=i+1;
		if(nextloc==pts1.size())
			nextloc=0;
		line_draw(pt[i], pt[nextloc]);
	}
	glFlush();
}
#pragma endregion OpenGL/glut drawing functions
#pragma endregion Structures and Global Variables 
#pragma region File_IO 
void read_file_data()
{
	ifstream ifs ( "data.txt" , ifstream::in);
	float a;
	int n=0;int i=0;
	vector<float> v1;
	vector<float> v2;
	for ( i; ifs.good(); i++)
	{
		ifs >> a;
		if(i==0)
			n=a;
		if(i<n*2+1 && i!=0)
		{
			v1.push_back(a);
		}
		else if(i!=0)
		{
			v2.push_back(a);
		}

	}
	ifs.close();

	for(int i=0; i<v1.size()-1; i++)
	{
		if(!(i%2))
		{
			Point tmp={v1[i], v1[i+1]};
			pts1.push_back(tmp);
			tmp.x=v2[i]; tmp.y=v2[i+1];
			pts2.push_back(tmp);
		}
	}
}
#pragma endregion File I/O 
#pragma region Event_Handlers
bool flip=false;
void keyboard (unsigned char key, int x, int y) {

	

}
#pragma endregion Event (KeyUp, Mouse, etc) Handlers
int main(int argc,char **argv)
{
	// Initialize Vectors pts1, pts2
	read_file_data();
	Transition_Object a(pts1, pts2);
	for(int x=0; x<pts1.size(); x++)
	{
		t.a[x]=pts1[x];
		t.b[x]=pts2[x];
	}
	glutInit(&argc,argv);
	glutInitWindowSize(LENGTH, WIDTH);
	//glutInitWindowPosition(100,150);
	glutCreateWindow("Tweening");
	glutDisplayFunc(clear);
	glutKeyboardUpFunc(keyboard);
	glClearColor(1.0,1.0,1.0,0.0);
	init();
	glutMainLoop();
}
