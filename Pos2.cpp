#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

// Globals
static float angleX = 30.0f;
static float angleY = 45.0f;
static float zoom = -25.0f;
int selectedGateIndex = -1;

struct Gates {
    enum { CLOSED, OPENED } state;
    bool selected;
    float x, y, z; // Posisi
    float angle;
    float speed;
    float yaw; // rotasi sumbu y
};

static Gates gates[4] = {
    // Kanan: Tambah 10 unit pada X
        {Gates::CLOSED, false, 12.7f, -1.4f, 0.4f, 0.0f, 90.0f, 0.0f},   // Kanan Depan
        {Gates::CLOSED, false, 11.2f, -1.4f, -3.0f, 0.0f, 90.0f, 180.0f}, // Kanan Belakang
        // Kiri: Kurang 10 unit dari X
        {Gates::CLOSED, false, -11.4f, -1.4f, -0.4f, 0.0f, 90.0f, 0.0f},  // Kiri Depan
        {Gates::CLOSED, false, -12.7f, -1.4f, -3.0f, 0.0f, 90.0f, 180.0f} // Kiri Belakang
};



//////////////////////////////////////
// Blueprint Pos Satpam
/////////////////////////////////////


#pragma region Draw Pos

// Fungsi menggambar kubus/dinding abu pos satpam
void drawColoredCube(float width, float height, float depth,
    float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_QUADS);

    // Front face
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

// Helper cube tanpa warna
void drawCubeNoColor(float width, float height, float depth)
{
    glBegin(GL_QUADS);
    // Front
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    // Back
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, -depth / 2);
    // Top
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    // Bottom
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    glVertex3f(-width / 2, -height / 2, depth / 2);
    // Right
    glVertex3f(width / 2, -height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, -depth / 2);
    glVertex3f(width / 2, height / 2, depth / 2);
    glVertex3f(width / 2, -height / 2, depth / 2);
    // Left
    glVertex3f(-width / 2, -height / 2, -depth / 2);
    glVertex3f(-width / 2, -height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, depth / 2);
    glVertex3f(-width / 2, height / 2, -depth / 2);
    glEnd();
}

// Fungsi untuk menggambar jendela
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

// Fungsi untuk menggambar pintu
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
// Fungsi Terop/Tenda
//=====================================

// Blueprint tiang penyangga terop
void drawCanopyPole(float x, float y, float z, float height)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    // Warna tiang besi
    drawColoredCube(0.3f, height, 0.3f, 0.1f, 0.1f, 0.3f); 
    glPopMatrix();
}

// Blueprint atap kain terop
void drawCanopyRoof(float width, float depth, float height)
{
    glPushMatrix();
    glTranslatef(0.0f, height, 0.0f);

    // Warna kain terop
    glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    // Atap atas (sedikit miring untuk efek kain)
    glVertex3f(-width / 2, 0.0f, -depth / 2);
    glVertex3f(width / 2, 0.0f, -depth / 2);
    glVertex3f(width / 2, 0.3f, depth / 2);
    glVertex3f(-width / 2, 0.3f, depth / 2);
    glEnd();

    // Sisi-sisi terop (valance/border)
    glColor3f(0.6f, 0.6f, 0.6f);

    // Sisi depan
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0.3f, depth / 2);
    glVertex3f(width / 2, 0.3f, depth / 2);
    glVertex3f(width / 2, -0.5f, depth / 2);
    glVertex3f(-width / 2, -0.5f, depth / 2);
    glEnd();

    // Sisi belakang
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0.0f, -depth / 2);
    glVertex3f(-width / 2, -0.5f, -depth / 2);
    glVertex3f(width / 2, -0.5f, -depth / 2);
    glVertex3f(width / 2, 0.0f, -depth / 2);
    glEnd();

    // Sisi kiri
    glBegin(GL_QUADS);
    glVertex3f(-width / 2, 0.0f, -depth / 2);
    glVertex3f(-width / 2, 0.3f, depth / 2);
    glVertex3f(-width / 2, -0.5f, depth / 2);
    glVertex3f(-width / 2, -0.5f, -depth / 2);
    glEnd();

    // Sisi kanan
    glBegin(GL_QUADS);
    glVertex3f(width / 2, 0.0f, -depth / 2);
    glVertex3f(width / 2, -0.5f, -depth / 2);
    glVertex3f(width / 2, -0.5f, depth / 2);
    glVertex3f(width / 2, 0.3f, depth / 2);
    glEnd();

    glPopMatrix();
}

// Fungsi untuk RENDER / menggambar seluruh terop/tenda
void drawCanopy()
{
    float canopyWidth = 26.0f;
    float canopyDepth = 24.0f;
    float poleHeight = 12.0f;
    float roofHeight = 8.0f;

    // 4 Tiang penyangga di keempat sudut
    // Tiang kiri depan
    drawCanopyPole(-canopyWidth / 2 + 0.5f, -4.0f + poleHeight / 2, canopyDepth / 2 - 0.5f, poleHeight);

    // Tiang kanan depan
    drawCanopyPole(canopyWidth / 2 - 0.5f, -4.0f + poleHeight / 2, canopyDepth / 2 - 0.5f, poleHeight);

    // Tiang kiri belakang
    drawCanopyPole(-canopyWidth / 2 + 0.5f, -4.0f + poleHeight / 2, -canopyDepth / 2 + 0.5f, poleHeight);

    // Tiang kanan belakang
    drawCanopyPole(canopyWidth / 2 - 0.5f, -4.0f + poleHeight / 2, -canopyDepth / 2 + 0.5f, poleHeight);

    // Atap kain terop
    drawCanopyRoof(canopyWidth, canopyDepth, roofHeight);
}

//=====================================
// Render dan transformasi Pos Satpam
//======================================

// Fungsi untuk menggambar pos satpam
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
    drawColoredCube(10.1f, 2.5f, 8.1f, 0.0f, 0.2f, 0.8f);
    glPopMatrix();

    // 3. Counter merah
    glPushMatrix();
    glTranslatef(0.0f, -1.1f, 0.0f);
    drawColoredCube(10.2f, 0.4f, 8.2f, 0.8f, 0.1f, 0.1f);
    glPopMatrix();

    // 4. Dinding abu-abu
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    drawColoredCube(10.0f, 6.0f, 8.0f, 0.7f, 0.7f, 0.7f);
    glPopMatrix();

    ////////////////////////////////////////////////////////////////
    // 5. PENEMPATAN JENDELA DAN PINTU
    ////////////////////////////////////////////////////////////////

    // SISI DEPAN (Z = 4.0f)
    // 5.1. Jendela depan kiri
    glPushMatrix();
    glTranslatef(-2.5f, 1.4f, 4.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    // 5.2. Jendela depan kanan
    glPushMatrix();
    glTranslatef(2.5f, 1.4f, 4.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    // SISI KANAN (X = 5.0f)
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // 5.3. Jendela rightside kiri
    glPushMatrix();
    glTranslatef(-1.5f, 1.4f, 5.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    // 5.4. Jendela rightside kanan
    glPushMatrix();
    glTranslatef(1.5f, 1.4f, 5.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    glPopMatrix();

    // SISI KIRI (X = -5.0f)
    glPushMatrix();
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    // 5.5. Jendela leftside kiri
    glPushMatrix();
    glTranslatef(-1.5f, 1.4f, 5.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    // 5.6. Jendela leftside kanan
    glPushMatrix();
    glTranslatef(1.5f, 1.4f, 5.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    glPopMatrix();

    // SISI BELAKANG (Z = -4.0f)
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    // 5.7. Jendela belakang kiri
    glPushMatrix();
    glTranslatef(-2.5f, 1.4f, 4.01f);
    drawWindow(3.8f, 3.8f);
    glPopMatrix();

    glPopMatrix();

    // PINTU
    // 6. Pintu samping kanan
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glTranslatef(1.9f, -2.7f, 4.01f);
    drawDoor(3.5f, 6.0f);
    glPopMatrix();

    // STRIP DAN ATAP
    // 7. Strip hijau
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    drawColoredCube(11.0f, 0.4f, 9.0f, 0.2f, 0.7f, 0.3f);
    glPopMatrix();

    // 8. Atap biru
    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    drawColoredCube(12.0f, 0.6f, 10.0f, 0.0f, 0.2f, 0.9f);
    glPopMatrix();

    // Banner POS SATPAM
    glPushMatrix();
    glTranslatef(-6.0f, 5.0f, 9.02f);
    glColor3f(0.0f, 0.0f, 0.0f);
    drawColoredCube(10.0f, 5.0f, 0.2f, 0.424f, 0.043f, 0.663f);
    glEnd();
    glPopMatrix();

    glPopMatrix();
}
#pragma endregion


//==================================
// Render dan transformasi Palang
//==================================

#pragma region Draw Palang
void drawBodyPalang()
{
    // KANAN
    // 1. Platform/Lantai bawah (hitam) kanan
    glPushMatrix();
    glTranslatef(12.0f, -4.0f, 0.0f);
    drawColoredCube(3.0f, 0.3f, 6.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    // Bagian bawah scan kTM
    glPushMatrix();
    glTranslatef(12.0f, -2.5f, 1.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 2.0f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // Bagian atas scan KTM
    glPushMatrix();
    glTranslatef(12.0f, 0.0f, 1.5f);
    drawColoredCube(2.5f, 2.0f, 2.0f, 0.8f, 0.0f, 0.0f);
    glPopMatrix();

    //Bagian pilar palang depan
    glPushMatrix();
    glTranslatef(12.0f, -2.5f, -0.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    //Bagian pilar palang belakang
    glPushMatrix();
    glTranslatef(12.0f, -2.5f, -2.2f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // KIRI
    // 1. Platform/Lantai bawah (hitam) kiri
    glPushMatrix();
    glTranslatef(-12.0f, -4.0f, 0.0f);
    drawColoredCube(3.0f, 0.3f, 6.0f, 0.2f, 0.2f, 0.2f);
    glPopMatrix();

    // Bagian bawah scan kTM
    glPushMatrix();
    glTranslatef(-12.0f, -2.5f, 1.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 2.0f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    // Bagian atas scan KTM
    glPushMatrix();
    glTranslatef(-12.0f, 0.0f, 1.5f);
    drawColoredCube(2.5f, 2.0f, 2.0f, 0.8f, 0.0f, 0.0f);
    glPopMatrix();

    //Bagian pilar palang depan
    glPushMatrix();
    glTranslatef(-12.0f, -2.5f, -0.5f);
    glScalef(1.0f, 1.0f, 1.0f);
    drawColoredCube(2.0f, 3.0f, 1.5f, 0.5f, 0.5f, 0.5f);
    glPopMatrix();

    //Bagian pilar palang belakang
    glPushMatrix();
    glTranslatef(-12.0f, -2.5f, -2.2f);
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

    if (selected) glColor3f(1.0f, 1.0f, 1.0f);
    else          glColor3f(0.0f, 0.0f, 1.0f);

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
    GLboolean wasDither = glIsEnabled(GL_DITHER);
    GLboolean wasBlend = glIsEnabled(GL_BLEND);

    glDisable(GL_LIGHTING);
    glDisable(GL_DITHER);
    glDisable(GL_BLEND);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    for (int i = 0; i < 4; ++i) {
        glColor3ub((unsigned char)(i + 1), 0, 0);
        glPushMatrix();
        glTranslatef(gates[i].x, gates[i].y + 1.2f, gates[i].z);
        drawCubeNoColor(6.5f, 4.0f, 6.5f);
        glPopMatrix();
    }
    glFinish();

    int vp[4]; glGetIntegerv(GL_VIEWPORT, vp);
    unsigned char px[3] = { 0,0,0 };
    glReadPixels(mx, vp[3] - my - 1, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, px);

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
        Gates& g = gates[i];
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
    float dt = (current - prevMS) / 1000.0f;
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

    // Gambar terop/tenda terlebih dahulu
    drawCanopy();

    // Kemudian pos satpam
    drawPosSatpam();

    // Terakhir palang
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
        zoom = -25.0f;
        break;
    case ' ':
        if (selectedGateIndex >= 0) {
            Gates& g = gates[selectedGateIndex];
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
    std::cout << "=== POS SATPAM 3D DENGAN TEROP ===" << std::endl;
    std::cout << "Kontrol:" << std::endl;
    std::cout << "- W/S atau Arrow Up/Down: Rotasi X" << std::endl;
    std::cout << "- A/D atau Arrow Left/Right: Rotasi Y" << std::endl;
    std::cout << "- +/-: Zoom in/out" << std::endl;
    std::cout << "- SPASI: Buka/Tutup palang yang dipilih" << std::endl;
    std::cout << "- KLIK: Pilih palang" << std::endl;
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
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pos Satpam 3D dengan Terop - OpenGL");

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