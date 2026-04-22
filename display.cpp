#include "raster_algorithms.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib>

using namespace std;

extern vector<AlgorithmStep> algorithmSteps;

// ── Text helpers ──────────────────────────────────────────────────────────
void renderText(float x, float y, const char* text, void* font)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c; c++)
        glutBitmapCharacter(font, *c);
}

// ── Algorithm step table (Line algorithms only) ───────────────────────────
void displayAlgorithmSteps()
{
    if (algorithmSteps.empty()) return;

    float sx = -ORTHO_W + 2.0f;
    float sy =  ORTHO_H - 15.0f;

    // Title
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(sx, sy);
    const char* title = (currentAlgo == 1)
        ? "BRESENHAM LINE  Step / X / Y / P"
        : "MIDPOINT LINE   Step / X / Y / D";
    for (const char* c = title; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    // Header
    glColor3f(0.0f, 1.0f, 1.0f);
    glRasterPos2f(sx, sy - 10.0f);
    const char* hdr = "Step    X       Y       P/D";
    for (const char* c = hdr; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);

    // Rows
    int show = min((int)algorithmSteps.size(), 22);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < show; i++)
    {
        const auto& s = algorithmSteps[i];
        ostringstream ss;
        ss << setw(3) << s.step << "    "
           << setw(5) << s.x   << "    "
           << setw(5) << s.y   << "    "
           << setw(6) << s.p;
        glRasterPos2f(sx, sy - 20.0f - i * 6.8f);
        for (char c : ss.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }

    if ((int)algorithmSteps.size() > show)
    {
        glColor3f(0.6f, 0.6f, 0.6f);
        ostringstream more;
        more << "... +" << (algorithmSteps.size() - show) << " more steps";
        glRasterPos2f(sx, sy - 20.0f - show * 6.8f);
        for (char c : more.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }
}

// ── Coordinate list sidebar (circles) ────────────────────────────────────
void drawPlottedCoordinates()
{
    if (plottedPoints.empty()) return;

    float sx = -ORTHO_W + 2.0f;
    float sy =  ORTHO_H - 15.0f;

    glColor3f(0.6f, 0.95f, 0.6f);
    glRasterPos2f(sx, sy);
    const char* hdr = "Plotted Points (first 22):";
    for (const char* c = hdr; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    int show = min((int)plottedPoints.size(), 22);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < show; i++)
    {
        ostringstream ss;
        ss << "(" << plottedPoints[i].x << ", " << plottedPoints[i].y << ")";
        glRasterPos2f(sx, sy - 10.0f - i * 6.8f);
        for (char c : ss.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }

    if ((int)plottedPoints.size() > show)
    {
        glColor3f(0.6f, 0.6f, 0.6f);
        ostringstream more;
        more << "... +" << (plottedPoints.size() - show) << " more";
        glRasterPos2f(sx, sy - 10.0f - show * 6.8f);
        for (char c : more.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }
}

// ── Grid ──────────────────────────────────────────────────────────────────
void drawGrid()
{
    // Minor grid lines
    glColor3f(0.13f, 0.13f, 0.18f);
    glBegin(GL_LINES);
    for (int i = -100; i <= 100; i += 10)
    {
        glVertex2i(i, -100); glVertex2i(i,  100);  // vertical
        glVertex2i(-100, i); glVertex2i( 100, i);  // horizontal
    }
    glEnd();

    // Full-window axes
    glColor3f(0.35f, 0.35f, 0.45f);
    glBegin(GL_LINES);
        glVertex2f(-ORTHO_W, 0); glVertex2f(ORTHO_W, 0);
        glVertex2f(0, -ORTHO_H); glVertex2f(0,  ORTHO_H);
    glEnd();

    // Tick marks
    glColor3f(0.45f, 0.45f, 0.55f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (int i = -100; i <= 100; i += 10)
    {
        glVertex2i(i, 0);
        glVertex2i(0, i);
    }
    glEnd();

    // Axis labels
    glColor3f(0.55f, 0.55f, 0.65f);
    for (int i = -100; i <= 100; i += 10)
    {
        if (i == 0) continue;
        ostringstream ss;
        ss << i;
        glRasterPos2i(i - 5, -9);
        for (char c : ss.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);

        ss.str(""); ss << i;
        glRasterPos2i(-18, i - 3);
        for (char c : ss.str())
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, c);
    }
    glRasterPos2i(-6, -9);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, '0');
}

// ── Main display callback ─────────────────────────────────────────────────
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawGrid();

    if (currentAlgo == 0) { glutSwapBuffers(); return; }

    // Draw algorithm output
    switch (currentAlgo)
    {
        case 1:  // Bresenham Line — green
            glColor3f(0.0f, 1.0f, 0.2f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_STRIP);
            for (const auto& pt : plottedPoints) glVertex2i(pt.x, pt.y);
            glEnd();
            glLineWidth(1.0f);
            break;

        case 2:  // Midpoint Line — red
            glColor3f(1.0f, 0.2f, 0.2f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_STRIP);
            for (const auto& pt : plottedPoints) glVertex2i(pt.x, pt.y);
            glEnd();
            glLineWidth(1.0f);
            break;

        case 3:  // Bresenham Circle — magenta
            glColor3f(1.0f, 0.1f, 1.0f);
            glPointSize(3.0f);
            glBegin(GL_POINTS);
            for (const auto& pt : plottedPoints) glVertex2i(pt.x, pt.y);
            glEnd();
            break;

        case 4:  // Midpoint Circle — cyan
            glColor3f(0.1f, 1.0f, 1.0f);
            glPointSize(3.0f);
            glBegin(GL_POINTS);
            for (const auto& pt : plottedPoints) glVertex2i(pt.x, pt.y);
            glEnd();
            break;
    }

    // Sidebar
    if (currentAlgo == 1 || currentAlgo == 2) displayAlgorithmSteps();
    else                                        drawPlottedCoordinates();

    // Algorithm label — top centre
    const char* labels[] = {
        "", "1. Bresenham Line", "2. Midpoint Line",
        "3. Bresenham Circle", "4. Midpoint Circle"
    };
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-22.0f, ORTHO_H - 5.0f);
    const char* lbl = labels[currentAlgo];
    while (*lbl) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *lbl++);

    // Bottom hint
    glColor3f(0.5f, 0.5f, 0.5f);
    glRasterPos2f(-ORTHO_W + 2.0f, -ORTHO_H + 3.0f);
    const char* hint = "ESC = back to menu";
    while (*hint) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *hint++);

    glutSwapBuffers();
}

// ── Keyboard ──────────────────────────────────────────────────────────────
void keyboard(unsigned char key, int, int)
{
    cout << "Key pressed: " << (int)key << " (ESC=" << 27 << ")" << endl;
    
    if (key == 27)  // ESC key
    {
        cout << "ESC detected! Closing window and returning to menu..." << endl;
        glutDestroyWindow(glutGetWindow());
        runMenu();
    }
}

// ── Helper: open a GL window and enter its loop ───────────────────────────
static void openGLWindow()
{
    cout << "  Creating OpenGL window..." << endl;
    
    glutInitWindowSize(WIN_W, WIN_H);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Raster Drawing Algorithms - Bresenham & Midpoint");

    glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-ORTHO_W, ORTHO_W, -ORTHO_H, ORTHO_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    
    cout << "  OpenGL window created. Press ESC in the WINDOW to return to menu." << endl;
    glutMainLoop();
}

// ── Terminal menu ─────────────────────────────────────────────────────────
void runMenu()
{
    while (true)
    {
        cout << "\n==============================================" << endl;
        cout << "  Raster Drawing Algorithms" << endl;
        cout << "==============================================" << endl;
        cout << "  1. Bresenham Line Algorithm" << endl;
        cout << "  2. Midpoint Line Algorithm" << endl;
        cout << "  3. Bresenham Circle Algorithm" << endl;
        cout << "  4. Midpoint Circle Algorithm" << endl;
        cout << "  0. Exit" << endl;
        cout << "==============================================" << endl;
        cout << "  Select (0-4): ";

        int choice = -1;
        cin >> choice;

        if (choice == 0) { cout << "Goodbye!" << endl; exit(0); }
        if (choice < 1 || choice > 4)
        {
            cout << "  Invalid. Enter 0-4." << endl;
            continue;
        }

        currentAlgo = choice;
        plottedPoints.clear();
        algorithmSteps.clear();

        // Collect input and run algorithm
        if (choice == 1 || choice == 2)
        {
            cout << "\n  Enter X1: "; cin >> lineParams.x1;
            cout << "  Enter Y1: "; cin >> lineParams.y1;
            cout << "  Enter X2: "; cin >> lineParams.x2;
            cout << "  Enter Y2: "; cin >> lineParams.y2;

            if (choice == 1)
                bresenhamsLine(lineParams.x1, lineParams.y1,
                               lineParams.x2, lineParams.y2);
            else
                midpointLine(lineParams.x1, lineParams.y1,
                             lineParams.x2, lineParams.y2);
        }
        else
        {
            cout << "\n  Enter CX: "; cin >> circleParams.cx;
            cout << "  Enter CY: "; cin >> circleParams.cy;
            cout << "  Enter R:  "; cin >> circleParams.r;

            if (choice == 3)
                bresenhamsCircle(circleParams.cx, circleParams.cy,
                                 circleParams.r);
            else
                midpointCircle(circleParams.cx, circleParams.cy,
                               circleParams.r);
        }

        cout << "\n  Opening window... (ESC to return here)" << endl;
        openGLWindow();   // blocks until ESC
    }
}