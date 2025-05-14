#include "../headers.h"
#include <GL/glut.h>

void display (){
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0,5.0);
    glVertex2f(4.0,-3.0);
    glVertex2f(-4,-3.0);
    glEnd();
    glFlush();
}