#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 



int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("parkiranktm.cpp");

    glewExperimental = GL_TRUE;
    glewInit();

    return 0;
}