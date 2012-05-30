// Andre Brutus
#include <iostream>
#include <cmath>
//#include <cstdlib>
#include <sstream>

//  Adjust as appropriate
//   For Windows/Linux
#include <GL/glut.h>
//  For Mac OS X
//#include <GLUT/glut.h>

//  Fractal boxes program

using namespace std;

//  Constants representing the window and viewport extents
const int MIN_X = 0,
          MAX_X = 600,
          MIN_Y = 0,
          MAX_Y = 600;

//  A point within the graphics window
struct Point
{
    double x, y;
    Point(double x, double y): x(x), y(y) {}
};

//  Global value for the current depth limit of the recursion
//  that renders the fractal boxes.
int current_depth = 3;

//  Global constant limiting the depth of the recursion
//  when rendering the fractal boxes. 
int MAX_DEPTH = 10;

//  Adjust the window's titlebar to disply the current 
//  recursion depth of the fractal generation.
void set_title()
{
    stringstream out;
    out << "Fractal Boxes Depth = " << current_depth;
    glutSetWindowTitle(out.str().c_str());
    
}

//  The common OpenGL initialization code
void init()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);  //  white background
    glShadeModel(GL_FLAT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(MIN_X, MAX_X, MIN_Y, MAX_Y);
    set_title();
}

//  Draw a rectangle with the corners
//
//   p0--------p1
//   |         |
//   |         |
//   |         |
//   p3--------p2
//  
void draw_box(const Point& p0, const Point& p1, 
              const Point& p2, const Point& p3)
{
    glBegin(GL_LINES);
	    glVertex2d(p0.x, p0.y);
	    glVertex2d(p1.x, p1.y);
	    glVertex2d(p1.x, p1.y);
	    glVertex2d(p2.x, p2.y);
	    glVertex2d(p2.x, p2.y);
	    glVertex2d(p3.x, p3.y);
	    glVertex2d(p3.x, p3.y);
	    glVertex2d(p0.x, p0.y);
	glEnd();
}

//  Compute the midpoint between points p0 and p1.
Point midpoint(const Point& p0, const Point& p1)
{
    return Point((p0.x + p1.x)/2.0, (p0.y + p1.y)/2.0);
}


//  Draw the fractal box with corners
//
//   p0--------p1
//   |         |
//   |         |
//   |         |
//   p3--------p2
//  
//  The function should recursively generate the self-similar
//  boxes, not just draw grid lines based the depth parameter.
//  The depth parameter indicates the depth of the recursion,
//  not to exceed the current depth.
// Andre Brutus
void draw_fractal_box(const Point& p0, const Point& p1, 
                      const Point& p2, const Point& p3, 
                      int depth)
{
	draw_box(p0, p1, p2, p3);
    if(depth > 0)
	{
		
		draw_fractal_box(p0, midpoint(p0,p1), midpoint(p0,p2), midpoint (p0,p3), depth-1);
		draw_fractal_box(midpoint(p0,p1), p1, midpoint(p1,p2), midpoint(p0,p2), depth-1);
		draw_fractal_box(midpoint(p0,p3),  midpoint(p0,p2), midpoint(p3,p2), p3, depth-1);
		draw_fractal_box(midpoint(p0,p2),  midpoint(p1,p2), p2, midpoint(p3,p2), depth-1);
	}
}

//  The display callback registered with OpenGL.
//  This function calls the recursive fractal box
//  drawing function.
void display()
{
    //  The corners of the big, outer box
    Point p0(MIN_X + 10, MAX_Y - 10),
          p1(MAX_X - 10, MAX_Y - 10),
          p2(MAX_X - 10, MIN_Y + 10),
          p3(MIN_X + 10, MIN_Y + 10);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    //  Plot the fractal boxes
	draw_fractal_box(p0, p1, p2, p3, current_depth);
    glFlush();
}

//  Code to execute when the user resizes the window.
void reshape(int w, int h)
{
    if ( w != h )
    {   //  force window to be square
        (w > h)? w = h : h = w;
        glutReshapeWindow(w, h);
    }
    glutInitWindowSize(w, h);
    glViewport (0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(MIN_X, MAX_X, MIN_Y, MAX_Y);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


//  The user adjusts the recursion depth with the arrow 
//  keys. Up arrow: increases the depth of recursion.
//        Down arrow: decreases the depth of recursion.
void process_key(int key, int, int)
{
    //  Arrow keys modify the depth of the recursion
	switch( key ) 
    {
		case GLUT_KEY_DOWN : 
            //  Fewer segments
            if ( current_depth > 0 )
                current_depth -= 1;
            break;
		case GLUT_KEY_UP : 
            //  More segments
            if ( current_depth < MAX_DEPTH )
                current_depth += 1;
            break;
    }
    set_title();
    if ( key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT )
        exit(0);   //  Alt-F4 terminates the program
    glutPostRedisplay();
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(MAX_X, MAX_Y);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Fractal Boxes");
    glutDisplayFunc(display);
	glutSpecialFunc(process_key);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
}
