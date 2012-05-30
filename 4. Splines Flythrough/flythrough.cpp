#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <GL/glut.h>
GLfloat **dynamicArray = 0;
int sizeofarray=0;
int currentPnt=0;
//  A generic 3-D point object
struct Point
{
	double x, y, z;
	Point(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
};

//  A generic 3-D vector object (not a C++ STL vector!)
struct Vector
{
	double x, y, z;
	Vector(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
	//  Reverse the vector
	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}
};
std::vector<Point> between;
//  Dimensions used when scaling the buildings
struct Size
{
	double width, height, depth;
	Size(double wd, double ht, double dp):
	width(wd), height(ht), depth(dp) {}
};

//  An RGB color object
struct Color
{
	float red, green, blue;
	Color(float r, float g, float b): red(r), green(g), blue(b) {}
};

//  A building in the fly-through scene
struct Building
{
	Point center;
	Size  size;
	Color color;

	//  Constructor
	Building(Point cen, Size s, Color col): 
	center(cen.x, cen.y, s.height/2.0), 
		size(s), color(col) {}

	void draw() const
	{

		//  Set properties of the surface materials
		GLfloat mat_ambient[] = { color.red, color.green, color.blue };
		GLfloat mat_diffuse[] = { color.red, color.green, color.blue };
		GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
		GLfloat mat_shininess[] = { 20.0f }; 
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
		glPushMatrix();
		glTranslated(center.x, center.y + size.height/2.0, center.z);
		glScaled(size.width, size.height, size.depth);
		glutSolidCube(1.0);
		glPopMatrix();
	}
};


//  Type of the collection of all the buildings in the scene
typedef std::vector<Building> Scene;  //  Collection of buildings
typedef std::vector<Point> PointList;
//  Global values keeping track of the viewer's current position
//  and heading
PointList splineload=PointList();
Point position(0.0, 0.2, -20.0);
Vector heading(0.0, 0.0, 0.01);
//  Initial heading toward the 
//  origin along the z axis

//  Loads the building data for the scene
void load_scene(Scene& scene, std::string filename)
{
	std::fstream fin(filename.c_str(), std::ios::in);
	if ( fin.good() )
	{
		scene.clear();
		int num_buildings;
		//  How many buildings?
		fin >> num_buildings;
		for ( int i = 0; i < num_buildings; i++ )
		{
			float cx, cy, cz, //  Point coordinates
				height, width, depth,  //  Dimensions
				red, green, blue;      //  Color components
			//  Read in information about a building
			fin >> cx >> cy >> cz
				>> height >> width >> depth
				>> red >> green >> blue;
			scene.push_back(Building(Point(cx, cy, cz), 
				Size(height, width, depth),
				Color(red, green, blue)));
		}
	}
	else
	{
		std::cout << "Could not open " << filename << std::endl;
		exit(1);  //  Terminate program in error state
	}

}
const std::string SPLINES = "splines.dat";

void load_splines()
{
	std::fstream fin(SPLINES.c_str(), std::ios::in);

	if ( fin.good() )
	{
		float num_buildings=0;

		fin >> num_buildings;
		for ( int i = 0; i < num_buildings; i++ )
		{
			float cx, cy, cz; //  Point coordinates

			//  Read in information about a building
			fin >> cx >> cy >> cz;
			splineload.push_back(Point(cx, cy, cz));
		}
		int size=splineload.size();

		//GLfloat **dynamicArray = 0;

		dynamicArray = new GLfloat *[size] ;


		for( int i = 0 ; i < size ; i++ )
		{
			dynamicArray[i] = new GLfloat[3];
			dynamicArray[i][0]= splineload[i].x;
			dynamicArray[i][1]= splineload[i].y;
			dynamicArray[i][2]= splineload[i].z;

		}
		sizeofarray=size;
	}
	else
	{
		std::cout << "Could not open " << SPLINES << std::endl;
		exit(1);  //  Terminate program in error state
	}
}

// Name of the data file containing the building data
const std::string FILENAME = "buildings.dat";


//  Data structure holding all the buildings in the scene
Scene scene;

//  Load the scene from the scene data file
void calculate_points();
void initialize()
{
	load_scene(scene, FILENAME);
	load_splines();
	calculate_points();
}

//  Render the buildings by iterating over all the buildings in the
//  scene
void draw_buildings(const Scene& scene)
{
	for ( unsigned i = 0; i < scene.size(); i++ )
		scene[i].draw();
}

//  Draw the ground
void ground(double thickness)
{ // draw thin floor with top = xz-plane, corner at origin
	//  Set properties of the surface materials
	GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.7f, 1.0f };  // gray
	GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.0f, 1.0f };  
	GLfloat mat_specular[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
	GLfloat mat_shininess[] = { 8.0f }; 
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glPushMatrix();
	//glTranslated(0.0, 0.0, 0.0);
	glScaled(40.0, thickness, 40.0);
	glutSolidCube(1.0);
	glPopMatrix();
}	


//  The display function registered with GLUT
void display()
{

	//  Set light source properties
	GLfloat light_intensity0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat light_position0[] = { 2.0f, 600.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity0);
	GLfloat light_intensity1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat light_position1[] = { 10.0f, 10.0f, 200.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_intensity1);
	//  Set the camera
	glMatrixMode(GL_PROJECTION); // set the view volume shape
	glLoadIdentity();
	//double win_ht = 1.0;  //  half-height of the window
	//glOrtho(-win_ht*64/48.0, win_ht*64/48.0, -win_ht, win_ht, 0.1, 100);
	gluPerspective(60.0, 640.0/480, 0.5, 100.0);

	glMatrixMode(GL_MODELVIEW); // position and aim the camera
	glLoadIdentity();

	//  Compute viewpoint
	double view_x = position.x + 3*heading.x, 
		view_y = position.y + 3*heading.y, 
		view_z = position.z + 3*heading.z;
	//std::cout << "heading = " << "<" << heading.x << "," <<
	//	heading.y << "," << heading.z << ">" << std::endl;
	gluLookAt(position.x, position.y, position.z, 
		0, 0, 0,
		0.0, 1.0, 0.0);

	glDepthMask(GL_TRUE);

	//  Start drawing

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen
	glPushMatrix();

	//andre trying to visualize the thing.. epic fail like 5 times
	//GLUnurbsObj *curveName;
	//curveName = gluNewNurbsRenderer();
	//GLfloat knotVector[50];
	//for(int i=0; i<50; i++)
	//{
	//	knotVector[i]=i/50.0;
	//}
	////GLfloat knotVector[6]={ 0.0, 0.0, 0.0, 1.0,	1.0,1.0};
	//gluBeginCurve(curveName);
	//gluNurbsCurve(curveName, 50, knotVector, 3, &dynamicArray[0][0], 4, GL_MAP1_VERTEX_3);
	//gluEndCurve(curveName);
	////glPopMatrix();
	//GLint k;
	//glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, sizeofarray, &dynamicArray[0][0]);
	//glEnable(GL_MAP1_VERTEX_3);

	////	glPushMatrix();
	//glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(1.0, 1.0, 1.0);
	//glBegin(GL_LINE_STRIP);
	//for (k = 0; k <= 699; k++) 
	//	glEvalCoord1f((GLfloat) k/699.0);

	//glEnd();
	//glColor3f(1.0,0.0,0.0);
	//glPointSize(20.0);
	//glBegin(GL_POINTS);
	//for(k=0; k<sizeofarray; k++)
	//	glVertex3fv(&dynamicArray[k][0]);
	//glEnd();
	//glBegin(GL_LINES);
	//for(k=0; k<sizeofarray-1; k++)
	//{
	//	glVertex3f(dynamicArray[k][0], dynamicArray[k][1], dynamicArray[k][2]);
	//	glVertex3f(dynamicArray[k+1][0], dynamicArray[k+1][1], dynamicArray[k+1][2]);
	//}
	//glEnd();
	////glPopMatrix();

	//andre
	glTranslated(2.0, 0.0, 2.0);//  First leg
	draw_buildings(scene);

	glPopMatrix();
	ground(0.02);       //  Draw ground in xz-plane

	//glutSwapBuffers();

	glFlush();

}
Point parameter(float t, int i)
{
	return Point(((-1.0f/6.0f*t*t*t + 0.5f*t*t - 0.5f*t + 1.0f/6.0f)* splineload[i-1].x) + 
				((0.5f*t*t*t + -1.0f*t*t + 0 + 4.0f/6.0f)* splineload[i].x) + ((-0.5f*t*t*t + 0.5f*t*t + 0.5f*t + 1.0f/6.0f)* splineload[i+1].x) + 
				((1.0f/6.0f*t*t*t + 0 + 0 + 0)* splineload[i+2].x),((-1.0f/6.0f*t*t*t + 0.5f*t*t - 0.5f*t + 1.0f/6.0f)     * splineload[i-1].y) + 
				((0.5f*t*t*t + -1.0f*t*t + 0 + 4.0f/6.0f)* splineload[i].y) + ((-0.5f*t*t*t + 0.5f*t*t + 0.5f*t + 1.0f/6.0f)* splineload[i+1].y) + 
				((1.0f/6.0f*t*t*t + 0 + 0 + 0)* splineload[i+2].y),((-1.0f/6.0f*t*t*t + 0.5f*t*t - 0.5f*t + 1.0f/6.0f)     * splineload[i-1].z) + 
				((0.5f*t*t*t + -1.0f*t*t + 0 + 4.0f/6.0f)* splineload[i].z) +  ((-0.5f*t*t*t + 0.5f*t*t + 0.5f*t + 1.0f/6.0f)* splineload[i+1].z) + 
				((1.0f/6.0f*t*t*t + 0 + 0 + 0)* splineload[i+2].z));
}
void calculate_points()
{
	int fps = 20;
	float steps = 1.0f / fps;    
	between.push_back(splineload[0]);
	int size = splineload.size() - 2;
	for (int i = 1; i < size; i++)
	{
		float t = 0.0;
		//between.push_back(splineload.at(i));
		for (int j = 0; j < fps; j++)
		{
			between.push_back(parameter(t, i));

			t += steps;
		}
	}

	between.push_back(splineload[splineload.size() - 2]);
	between.push_back(splineload[splineload.size() - 1]);
}
//  Some handy precomputed constants to reduce the floating-point
//  computations at runtime
const double angle = 0.02,
	COS = cos(angle), SIN = sin(angle),
	NEG_COS = cos(-angle), NEG_SIN = sin(-angle);

//  Rotate the given vector around the y-axis
Vector rotate_y(const Vector& vec, int direction)
{
	double cosine = (direction)? COS : NEG_COS,
		sine   = (direction)? SIN : NEG_SIN;
	return Vector(vec.x*cosine - vec.z*sine, 
		vec.y, 
		vec.x*sine + vec.z*cosine);

}

//  Function registered with GLUT to respond to the user's
//  keyboard input
void process_key(int key, int, int)
{
	int size = between.size();
	//  Arrow keys modify heading vector
	//  and move forward in that direction
	Vector v(heading.x, heading.y, heading.z);
	double factor = 10.0;
	if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
		factor = 1.0;
	switch( key ) 
	{
	case GLUT_KEY_RIGHT : 
	case GLUT_KEY_UP : 
		currentPnt++;
		if (currentPnt >= size)
			currentPnt = 0;
		position.x = between[currentPnt].x;
		position.y =  between[currentPnt].y;
		position.z =  between[currentPnt].z;
		break;
	case GLUT_KEY_LEFT : 
	case GLUT_KEY_DOWN : 
		currentPnt--;
		if (currentPnt < 0)
			currentPnt = between.size()-1;
		position.x = between[currentPnt].x;
		position.y =  between[currentPnt].y;
		position.z =  between[currentPnt].z;
		break;
	case GLUT_KEY_PAGE_UP : 
		position.y += 0.1;
		break;
	case GLUT_KEY_PAGE_DOWN : 
		if ( position.y > 0.1 )
			position.y -= 0.1;
		break;
	}
	heading = v;
	if ( key == GLUT_KEY_F2 )
		if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT )
			heading = -heading;

	if ( key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT )
		exit(0);   //  Alt-F4 terminates the program
	glutPostRedisplay();
}

//  Currently not doing anything
//  Available for future expansion
void mouse_moved(int, int)
{
	glutPostRedisplay();
}

//  Currently not doing anything
//  Available for future expansion
void mouse_pressed(int button, int state, int, int)
{
	switch ( button ) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
		}
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("3D Scene");
	initialize();
	glutDisplayFunc(display);
	glutSpecialFunc(process_key);
	glutMotionFunc(mouse_moved);
	glutMouseFunc(mouse_pressed);
	glEnable(GL_LIGHTING);  //  Enable light sources
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);  //  for hidden surface removal
	glEnable(GL_NORMALIZE);   //  normalize vectors for proper shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // background is black
	glViewport(0, 0, 640, 480);
	glutMainLoop();
}
