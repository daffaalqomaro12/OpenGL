#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// Globals
static float angleX = 30.0f;
static float angleY = 45.0f;
static float zoom = -15.0f;
int selectedGateIndex = -1;

struct Gates{
    enum {CLOSED, OPENED} state;
    bool selected;
    float x, y, z; // Posisi
    float angle;
    float speed;
    float yaw; // rotasi sumbu y
};

static Gates gates[4] = {
    {Gates::CLOSED, false, 10.7f, -1.4f, 0.4f, 0.0f, 90.0f, 0.0f}, // Kanan Depan
    {Gates::CLOSED, false, 9.2f, -1.4f, -3.0f, 0.0f, 90.0f, 180.0f}, // Kanan Belakang
    {Gates::CLOSED, false, -9.4f, -1.4f, -0.4f, 0.0f, 90.0f, 0.0f}, // Kiri Depan
    {Gates::CLOSED, false, -10.7f, -1.4f, -3.0f, 0.0f, 90.0f, 180.0f} // Kiri Belakang
};


//////////////////////////////////////

// Blueprint Pos Satpam

/////////////////////////////////////


#pragma region Draw Pos

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
// Tambahkan helper ini
void drawCubeNoColor(float width, float height, float depth)
{
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-width/2, -height/2,  depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    // Back
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2,  height/2, -depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    glVertex3f( width/2, -height/2, -depth/2);
    // Top
    glVertex3f(-width/2,  height/2, -depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    // Bottom
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f( width/2, -height/2, -depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    glVertex3f(-width/2, -height/2,  depth/2);
    // Right
    glVertex3f( width/2, -height/2, -depth/2);
    glVertex3f( width/2,  height/2, -depth/2);
    glVertex3f( width/2,  height/2,  depth/2);
    glVertex3f( width/2, -height/2,  depth/2);
    // Left
    glVertex3f(-width/2, -height/2, -depth/2);
    glVertex3f(-width/2, -height/2,  depth/2);
    glVertex3f(-width/2,  height/2,  depth/2);
    glVertex3f(-width/2,  height/2, -depth/2);
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

//=====================================

// Render dan transformasi Pos Satpam

//======================================


// Fungsi untuk menggambar pos satpam (Object Mode blender)
void drawPosSatpam()
{
    glPushMatrix();

    // 1. Platform/Lantai bawah (hitam)
    glPushMatrix();
    glTranslatef(0.0f, -4.0f, 0.0f);
    drawColoredCube(40.0f, 0.3f, 40.0f, 0.2f, 0.2f, 0.2f);
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
#pragma endregion


//==================================

// Render dan transformasi Palang

//==================================

// Fungsi untuk menggambar palang
#pragma region Draw Palang
void drawBodyPalang()
{
    // KANAN
    // 1. Platform/Lantai bawah (hitam) kanan
    glPushMatrix();
    glTranslatef(10.0f, -4.0f, 0.0f);
    drawColoredCube(3.0f, 0.3f, 6.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    // Bagian bawah scan kTM
    glPushMatrix();
    glTranslatef(10.0f, -2.5f, 1.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 2.0f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // Bagian atas scan KTM
    glPushMatrix();
    glTranslatef(10.0f, 0.0f, 1.5f);
    drawColoredCube(2.5f, 2.0f, 2.0f, 0.8f, 0.0f, 0.0f);
    glPopMatrix();

    //Bagian pilar palang depan
    glPushMatrix();
    glTranslatef(10.0f, -2.5f, -0.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    //Bagian pilar palang belakang
    glPushMatrix();
    glTranslatef(10.0f, -2.5f, -2.2f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // KIRI
    // 1. Platform/Lantai bawah (hitam) kiri
    glPushMatrix();
    glTranslatef(-10.0f, -4.0f, 0.0f);
    drawColoredCube(3.0f, 0.3f, 6.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    // Bagian bawah scan kTM
    glPushMatrix();
    glTranslatef(-10.0f, -2.5f, 1.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 2.0f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // Bagian atas scan KTM
    glPushMatrix();
    glTranslatef(-10.0f, 0.0f, 1.5f);
    drawColoredCube(2.5f, 2.0f, 2.0f, 0.8f, 0.0f, 0.0f);
    glPopMatrix();

    //Bagian pilar palang depan
    glPushMatrix();
    glTranslatef(-10.0f, -2.5f, -0.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    //Bagian pilar palang belakang
    glPushMatrix();
    glTranslatef(-10.0f, -2.5f, -2.2f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();
}

void drawLenganPalang(float x, float y, float z, float angle, bool selected, float yaw)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);
    glScalef(0.5f, 0.5f, 0.5f);
    glTranslatef(3.0f, 0.0f, 0.0f);

    if (selected) glColor3f(1.0f, 1.0f, 1.0f);     // putih saat selected
    else          glColor3f(0.0f, 0.0f, 1.0f);     // biru default

    // Pakai versi tanpa set warna internal agar warna di atas kepakai
    drawCubeNoColor(14.0f, 1.0f, 0.5f);
    glPopMatrix();
}

void drawPalang()
{
    drawBodyPalang();
    for (int i = 0; i < 4; ++i) {
        bool sel = (i == selectedGateIndex);
        drawLenganPalang(gates[i].x, gates[i].y, gates[i].z, gates[i].angle, sel, gates[i].yaw);
    }
}
#pragma endregion

#pragma region Selection & Animation
int SelectHit(int mx, int my)
{
    GLboolean wasLighting = glIsEnabled(GL_LIGHTING);
    GLboolean wasDither   = glIsEnabled(GL_DITHER);
    GLboolean wasBlend    = glIsEnabled(GL_BLEND);

    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1,0,0);
    glRotatef(angleY, 0,1,0);

    for (int i = 0; i < 4; ++i) {
        glColor3ub((unsigned char)(i + 1), 0, 0);
        glPushMatrix();
        glTranslatef(gates[i].x, gates[i].y + 1.2f, gates[i].z);
        // proxy AABB agak besar supaya kena saat lengan berputar
        drawCubeNoColor(6.5f, 4.0f, 6.5f);
        glPopMatrix();
    }
    glFinish(); 

    int vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
    unsigned char px[3] = {0,0,0};
    glReadPixels(mx, vp[3]-my-1, 1,1, GL_RGB, GL_UNSIGNED_BYTE, px);

    if (wasBlend)   glEnable(GL_BLEND);
    if (wasDither)  glEnable(GL_DITHER);
    if (wasLighting)glEnable(GL_LIGHTING);

    int id = (int)px[0] - 1;
    return (id >= 0 && id < 4) ? id : -1;
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int id = SelectHit(x, y);
        selectedGateIndex = id;
        glutPostRedisplay();
    }
}

void UpdateGates(float dt)
{
    for (int i = 0; i < 4; ++i) {
        Gates &g = gates[i];
        if (g.state == Gates::OPENED) {
            if (g.angle < 90.0f) {
                g.angle += g.speed * dt;
                if (g.angle > 90.0f) g.angle = 90.0f;
            }
        }
        else if (g.state == Gates::CLOSED) {
            if (g.angle > 0.0f) {
                g.angle -= g.speed * dt;
                if (g.angle < 0.0f) g.angle = 0.0f;
            }
        }
    }
}
#pragma endregion
#pragma region Setup Function
void onTimer(int)
{
    static int prevMS = glutGet(GLUT_ELAPSED_TIME);
    int current = glutGet(GLUT_ELAPSED_TIME);
    float dt = (current - prevMS) / 1000.0f; // detik
    prevMS = current;

    UpdateGates(dt);
    glutPostRedisplay();
    glutTimerFunc(16, onTimer, 0);
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
    drawPalang();

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
    case ' ':
        if (selectedGateIndex >= 0) {
            Gates &g = gates[selectedGateIndex];
            if (g.state == Gates::CLOSED) {
                g.state = Gates::OPENED;
                std::cout << "Palang " << selectedGateIndex << " dibuka\n";
            }
            else if (g.state == Gates::OPENED) {
                g.state = Gates::CLOSED;
                std::cout << "Palang " << selectedGateIndex << " ditutup\n";
            }
        }
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

#pragma endregion

//==================================

// Main

//==================================

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
    glutMouseFunc(mouse);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();
    glutTimerFunc(16, onTimer, 0);

    glutMainLoop();

    return 0;
}