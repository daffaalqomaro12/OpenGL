#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 


class Gate{
public:
    float angle;
    float speed;
    enum State {CLOSED, OPENING, OPENHOLD, CLOSING} state;
    float holdTimer;
    float holdMax;
} gate;



#pragma region Draw (Menggambar)
void drawUnitCube(){
    glutSolidCube(1.0);
}

void drawRectXZ(float w, float d){
    glPushMatrix();
    glScalef(w, 1.0f, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawGround(){
    glColor3f(0.85f, 0.85f, 0.85f);
    glPushMatrix();
    glTranslatef(0.0f, -0.001f, 0.0f);
    drawRectXZ(20.0f, 20.0f);
    glPopMatrix();
}

void drawPos()
{
    glColor3f(0.9f, 0.9f, 0.95f);
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 2.0f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawUnitCube();
    glPopMatrix();
}

void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 2.0f, 9.0f,   // eye: tengah jalan, ~2 m tinggi, agak jauh
        0.0f, 1.0f, 0.0f,   // center: lihat ke origin setinggi ~dada
        0.0f, 1.0f, 0.0f    // up
    );   
    // Draw ground
    drawGround();

    drawPos();

    glutSwapBuffers();
}
#pragma endregion

#pragma region Setup
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(55.0, (float)w/(float)h, 0.1, 200.0);

	glMatrixMode(GL_MODELVIEW);
}
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}
#pragma endregion


#pragma region Main
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
    setup();
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutMainLoop();

    return 0;
}
#pragma endregion