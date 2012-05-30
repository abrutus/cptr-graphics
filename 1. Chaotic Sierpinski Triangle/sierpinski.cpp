// Andre Brutus 
#include <iostream>
#include <GL/glut.h>
#include <time.h>
#include <sstream>
using namespace std;

#pragma region Global_Variables 

int counter=0; //counts clicks
int N=2048; // iterations of serpinski loop

int dragged_point=-1; // point 0-2 that is being dragged

// point structure
struct point {
	int x; 
	int y;
};
point init_points[3]; //initial 3 points

bool draggin=0; //are we dragging a point or not

point latest_drag_end = { -1, -1 }; //latest dragged ending coordinates
point seed; // for the loop


#pragma endregion Global_Variables
#pragma region Debug_Help
// Takes in a point struct and returns a string  (x,y)
string p2s(point a)
{
	stringstream nicer;
	nicer << "(" << a.x << "," << a.y << ")";
	return nicer.str();
}
#pragma endregion Debug_Help
#pragma region Drawing_Functions
// Empties the screen
void draw()
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
void drawpoint(point a)
{
	glVertex2i(a.x,480-a.y);
}

// Draws a 10x10 green square around the given point
void drawsquarearond(point a)
{
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
	glVertex2i(a.x-5,480-a.y-5);
	glVertex2i(a.x-5 ,480-a.y+5);
	glVertex2i(a.x+5,480-a.y+5);
	glVertex2i(a.x+5,480-a.y-5);
	glEnd();
	glFlush();
	glColor3f(0.0f,0.0f,0.0f);
}

// Draws a line between point a and point b
void line_draw(point a, point b)
{
	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
	glVertex2i(a.x, 480-a.y);
	glVertex2i(b.x ,480-b.y);
	glEnd();
	glFlush();
	//cout << "Drawing line from " << p2s(a) << "to " << p2s(b) << endl;
	glColor3f(0.0f,0.0f,0.0f);
}

/// Sierpinski's Triangle
/// http://computing.southern.edu/halterman/Courses/Fall2011/425/Assignments/sierpinski.html
void draw_triangle()
{
	cout << "Starting loop" << endl;
	glBegin(GL_POINTS);
	//Change cursor to busy
	glutSetCursor(GLUT_CURSOR_CYCLE);

	for (int i = 0; i < N; i++)
	{
		//  Select one of the three vertex points at random.

		int rp= rand() % 3;
		point p=init_points[rp];

		//  Compute the midpoint of the seed point and this
		//  randomly chosen point
		point mp= { (p.x+ seed.x)/2, (p.y + seed.y)/2};

		//  This new point becomes the new seed point  
		drawpoint(mp);

		seed = mp;
	}
	glEnd();
	glFlush();
	cout << "Ended loop" << endl;
	// Restore cursor
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	
}

#pragma endregion Drawing_Functions
#pragma region Loopers
// Draws 'squaresaround' all the 3 initial points
void drawsquarearound_allinitpoints()
{
	for(int i=0; i<3; i++)
	{
		drawsquarearond(init_points[i]);
	}
}

// Draws lines between all 3 initial points
void line_draw_allinitialpoints(int replace=-1)
{
	for(int i=0; i<3; i++)
	{
		point first=init_points[i];
		if(i==dragged_point)
			first=latest_drag_end;
		point second=init_points[(i+1)%3];
		if((i+1)%3==dragged_point)
			second=latest_drag_end;

		line_draw(first, second);
	}
}
#pragma endregion Loopers
#pragma region Event_Handlers 
/// Determines if the passed point is an intended drag action, that is
/// if it is within 10 pixels both x and y away from any of the initial points
/// and if it is, it saves the point which is closest to, and returns true
bool dragging_point(point a)
{
	for(int i=0; i<3; i++)
	{
		if(abs(a.x - init_points[i].x)<10 && abs(a.y - init_points[i].y)<10)
		{
			dragged_point=i;
			cout << "Dragging:"<< i << p2s(init_points[i]) <<" because its closeness to " << p2s(a) << endl;
			return true;
		}
	}
	counter=0;
	return false;
}

// If its a drag event, this function will save the latest drag coordinates
void drag_point(int x,int y)
{
	point a= {x,y};
	if(dragging_point(a) || draggin)
	{
		//cout << "dragging " << "("<<x<<","<<y<<")"<<std::endl;
		draggin=1;
			if(a.x>640)
			a.x=640;
		if(a.x<0)
			a.x=0;
		if(a.y>480)
			a.y=480;
		if(a.y<0)
			a.y=0;
		latest_drag_end.x=a.x; latest_drag_end.y=a.y;
	
		init_points[dragged_point]=latest_drag_end;
		
		draw();
		//line_draw_allinitialpoints(dragged_point);
		draw_triangle();
	}
	else
	{
		//cout << "OUTSIDE REGION"<<std::endl;
		draggin=0;
	}

}

// Mouse Logic, if its not dragging it counts and resets if its supposed to 
void mouse_point(int ,int state,int x,int y)
{
	if(draggin==1)
	{
		cout << "Point " << dragged_point << " " <<p2s(init_points[dragged_point]) 
			<<	"was dragged to  " << p2s(latest_drag_end) << endl;

		init_points[dragged_point]=latest_drag_end;
		draw();
		drawsquarearound_allinitpoints();
		//line_draw_allinitialpoints(dragged_point);
		draw_triangle();
		draggin=0;
		return;
	}
	if(state==1)
	{
		draggin=0; dragged_point=-1;

		point a= { x, y };	
		//realcount= counter /2;
		if(counter <3)
		{
			//Clean screen at restart
			if(counter==0) draw();

			std::cout<< counter  <<"-"<< counter << ".("<<x<<","<<y<<")"<<std::endl;
			seed = a;
			init_points[counter]= a;
			drawpoint(a);
			if(counter <3)
				drawsquarearond(a);
		}
		else if( counter ==3)
		{
			//line_draw_allinitialpoints();			
			draw_triangle();

		}
		else if(counter >3 )
		{

			if(0 || dragging_point(a))
			{
				cout << "I should be dragging!" << endl;

			}
			else
			{
				//Clear Contents 
				counter=-1; dragged_point=-1; draggin=0;
			}
		}

		counter++;
	}
}
void keyboard (unsigned char key, int x, int y) {
	if((key=='u' || key=='U') && N<1048576)
		N=N*2;
	else if((key=='d' || key=='D') && N>256)
		N=N/2;
	char buffer [22];
	_itoa_s(N,buffer, 10);
	glutSetWindowTitle(buffer);
	draw();
	drawsquarearound_allinitpoints();
	//line_draw_allinitialpoints(dragged_point);
	draw_triangle();

}
#pragma endregion Event_Handlers

// glut main
int main(int argc,char **argv)
{
	srand(static_cast<unsigned int>(time(0)));
	glutInit(&argc,argv);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(100,150);
	glutCreateWindow("Starting N: 2048");
	glutDisplayFunc(draw);
	glutKeyboardUpFunc(keyboard);
	glutMouseFunc(mouse_point);
	glutMotionFunc(drag_point);
	glClearColor(1.0,1.0,1.0,0.0);
	init();
	glutMainLoop();
}
