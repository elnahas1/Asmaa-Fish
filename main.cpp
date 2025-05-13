#include <GL/glut.h>
#include "headers.h"

int screenWidth;
int screenHight;

int main (int argc, char ** argv){
    // Initialize glut
    glutInit(& argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    // Get screen dimensions
    screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    screenHight = glutGet(GLUT_SCREEN_HEIGHT);

    // Initialize a window
    glutInitWindowSize(screenWidth, screenHight);
    glutInitWindowPosition(screenWidth / 2, screenHight / 2); 
    glutCreateWindow("Asmaa's Fish Game");

    // Call back functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // Main loop
    glutMainLoop();
    return 0;
}