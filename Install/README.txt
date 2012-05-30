Ensure that you have the necessary graphics files
Windows.

If you are working on your own Windows computer, you need to add four files:

glut32.dll
glut32.lib
glut.h
sgl.hpp

Copy the files to the following locations:

glut32.dll: put this in your c:\Program Files\Microsoft Visual Studio 10.0\VC\bin\ folder. (You will need to adjust the path if you installed Visual Studio elsewhere.)
glut32.lib: put this in your c:\Program Files\Microsoft Visual Studio 10.0\VC\lib\ folder. (Again, You will need to adjust the path if you installed Visual Studio elsewhere.)
glut.h: create a folder named GL in your c:\Program Files\Microsoft Visual Studio 10.0\VC\include\ folder. Copy glut.h and sgl.hpp into this new folder.


Adapted from: http://computing.southern.edu/halterman/Courses/Fall2010/124/Labs/lab07_F10.html