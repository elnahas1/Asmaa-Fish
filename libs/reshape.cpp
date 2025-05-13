#include <GL/glut.h>
#include "../headers.h"

void reshape (int width, int hight){
    glViewport(0,0,width,hight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-10,10,-10,10);
    glMatrixMode(GL_MODELVIEW);
}