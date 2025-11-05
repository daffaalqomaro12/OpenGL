#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// Globals
static float angleX = 30.0f;
static float angleY = 45.0f;
static float zoom = -15.0f;

// Fungsi menggambar kubus/dinding abu pos satpam (Edit mode blender)
void drawColoredCube(float width, float height, float depth,
    float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_QUADS);

    // Front face
    // Ada 4 karena untuk membuat sebuah face perlu 4 titik.
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);

    // Back face
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, -depth / 2);

    // Top face
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);

    // Bottom face
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(-width / 2, -height / 2, depth / 2);

    // Right face
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);

    // Left face
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);

    glEnd();
}

// Fungsi untuk menggambar jendela (Edit mode blender)
void drawWindow(float width, float height)
{
    // Frame putih
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, -height / 2, 0.01f);
    glVertex3f(width / 2, height / 2, 0.01f);
    glVertex3f(-width / 2, height / 2, 0.01f);
    glEnd();

    // Kaca (abu-abu transparan)
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2 + 0.1f, -height / 2 + 0.1f, 0.02f);
    glVertex3f(width / 2 - 0.1f, -height / 2 + 0.1f, 0.02f);
    glVertex3f(width / 2 - 0.1f, height / 2 - 0.1f, 0.02f);
    glVertex3f(-width / 2 + 0.1f, height / 2 - 0.1f, 0.02f);
    glEnd();
}

// Fungsi untuk menggambar pintu (Edit Mode Blender)
void drawDoor(float width, float height)
{
    // Frame putih
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0.0f, 0.01f);
    glVertex3f(width / 2, 0.0f, 0.01f);
    glVertex3f(width / 2, height, 0.01f);
    glVertex3f(-width / 2, height, 0.01f);
    glEnd();

    // Daun pintu (abu-abu)
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
    glVertex3f(-width / 2 + 0.1f, 0.1f, 0.02f);
    glVertex3f(width / 2 - 0.1f, 0.1f, 0.02f);
    glVertex3f(width / 2 - 0.1f, height - 0.1f, 0.02f);
    glVertex3f(-width / 2 + 0.1f, height - 0.1f, 0.02f);
    glEnd();
}

// Fungsi untuk menggambar pos satpam (Object Mode blender)
void drawPosSatpam()
{
    glPushMatrix();

    // 1. Platform/Lantai bawah (hitam)
    glPushMatrix();
    glTranslatef(0.0f, -4.0f, 0.0f);
    drawColoredCube(6.0f, 0.3f, 6.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    // 2. Base biru bawah
    glPushMatrix();
    glTranslatef(0.0f, -2.5f, 0.0f);
    drawColoredCube(5.5f, 2.5f, 5.5f, 0.0f, 0.2f, 0.8f);
    glPopMatrix();

    // 3. Counter merah
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.0f);
    drawColoredCube(5.8f, 0.4f, 5.8f, 0.8f, 0.1f, 0.1f);
    glPopMatrix();

    // 4. Dinding abu-abu
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    drawColoredCube(5.0f, 4.0f, 5.0f, 0.7f, 0.7f, 0.7f);
    glPopMatrix();

    // 5. Jendela-jendela depan
    // Jendela kiri atas
    glPushMatrix();
    glTranslatef(-1.5f, 2.8f, 2.51f);
    drawWindow(0.8f, 0.6f);
    glPopMatrix();

    // Jendela tengah atas
    glPushMatrix();
    glTranslatef(0.0f, 2.8f, 2.51f);
    drawWindow(2.0f, 0.6f);
    glPopMatrix();

    // Jendela kanan atas
    //glPushMatrix();
    //glTranslatef(1.8f, 2.8f, 2.51f);
    //drawWindow(0.8f, 0.6f);
    //glPopMatrix();

// Jendela kanan_Test revisi
    glPushMatrix();
    // 1. ROTASI: Putar 90 derajat di sekitar sumbu Y agar menghadap ke kanan.
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // 2. TRANSLASI: Geser ke posisi yang sesuai di dinding kanan.
    //    - Z = 2.51f (tetap di depan)
    //    - X = -1.5f (sekarang menjadi koordinat Z horizontal di dinding kanan)
    //    - Y = 2.8f (tetap ketinggian)
    // Disini "Kanan-kiri" adalah sumbu Z bukan X. Entah apa yang bikin gini..
    // X = depan-belakang, Y = atas-bawah, Z = Kanan-kiri
    glTranslatef(-1.5f, 2.8f, 4.0f); //Gerakkan posisi relatif (objek mode transformations)
    drawWindow(0.8f, 0.6f); //panggil dan bangun blueprint ke dunia 3d
    glPopMatrix();


    // Jendela besar tengah
    glPushMatrix();
    glTranslatef(-0.3f, 1.2f, 2.51f);
    drawWindow(2.5f, 2.2f);
    glPopMatrix();

    // Jendela kiri bawah
    glPushMatrix();
    glTranslatef(-1.8f, 1.5f, 2.51f);
    drawWindow(0.8f, 1.5f);
    glPopMatrix();

    // Jendela kiri bawah kecil
    glPushMatrix();
    glTranslatef(-1.8f, 0.2f, 2.51f);
    drawWindow(0.8f, 0.8f);
    glPopMatrix();

    // 6. Pintu samping kanan
    glPushMatrix();
    glTranslatef(1.9f, 1.0f, 2.51f);
    drawDoor(0.9f, 3.0f);
    glPopMatrix();

    // 7. Strip hijau
    glPushMatrix();
    glTranslatef(0.0f, 3.7f, 0.0f);
    drawColoredCube(5.2f, 0.4f, 5.2f, 0.2f, 0.7f, 0.3f);
    glPopMatrix();

    // 8. Atap biru
    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    drawColoredCube(6.0f, 0.6f, 6.0f, 0.0f, 0.2f, 0.9f);
    glPopMatrix();

    // Text "POS SATPAM" (simulasi dengan kotak hitam)
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 2.52f);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex3f(-0.8f, -0.2f, 0.0f);
    glVertex3f(0.8f, -0.2f, 0.0f);
    glVertex3f(0.8f, 0.0f, 0.0f);
    glVertex3f(-0.8f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}

// Setup pencahayaan
void setupLighting()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat lightPos[] = { 5.0f, 10.0f, 10.0f, 1.0f };
    GLfloat lightAmb[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpec[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
}

// Drawing routine
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    drawPosSatpam();

    glutSwapBuffers();
}

// Initialization
void setup()
{
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    setupLighting();
    glShadeModel(GL_SMOOTH);
}

// Reshape
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;
    case 'w':
    case 'W':
        angleX -= 5.0f;
        break;
    case 's':
    case 'S':
        angleX += 5.0f;
        break;
    case 'a':
    case 'A':
        angleY -= 5.0f;
        break;
    case 'd':
    case 'D':
        angleY += 5.0f;
        break;
    case '+':
    case '=':
        zoom += 0.5f;
        break;
    case '-':
    case '_':
        zoom -= 0.5f;
        break;
    case 'r':
    case 'R':
        angleX = 30.0f;
        angleY = 45.0f;
        zoom = -15.0f;
        break;
    }
    glutPostRedisplay();
}

// Special keys
void specialKeyInput(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        angleX -= 5.0f;
        break;
    case GLUT_KEY_DOWN:
        angleX += 5.0f;
        break;
    case GLUT_KEY_LEFT:
        angleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        angleY += 5.0f;
        break;
    }
    glutPostRedisplay();
}

// Print instructions
void printInteraction()
{
    std::cout << "=== POS SATPAM 3D ===" << std::endl;
    std::cout << "Kontrol:" << std::endl;
    std::cout << "- W/S atau Arrow Up/Down: Rotasi X" << std::endl;
    std::cout << "- A/D atau Arrow Left/Right: Rotasi Y" << std::endl;
    std::cout << "- +/-: Zoom in/out" << std::endl;
    std::cout << "- R: Reset posisi" << std::endl;
    std::cout << "- ESC: Keluar" << std::endl;
}

// Main
int main(int argc, char** argv)
{
    printInteraction();

    glutInit(&argc, argv);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pos Satpam 3D - OpenGL");

    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialKeyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();

    return 0;
}