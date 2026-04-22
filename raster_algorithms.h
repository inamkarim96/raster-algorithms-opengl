#ifndef RASTER_ALGORITHMS_H
#define RASTER_ALGORITHMS_H

#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <string>
using namespace std;
const int WIN_W = 1400;
const int WIN_H =  900;

#define ORTHO_H 120.0f
#define ORTHO_W (ORTHO_H * ((float)WIN_W / (float)WIN_H))

struct AlgorithmStep {
    int step, x, y, p;
    string decision;
};

struct PlottedPoint  { int x, y; };
struct LineParams    { int x1, y1, x2, y2; };
struct CircleParams  { int cx, cy, r; };

extern vector<AlgorithmStep> algorithmSteps;
extern vector<PlottedPoint>  plottedPoints;
extern LineParams                 lineParams;
extern CircleParams               circleParams;
extern int                        currentAlgo;
extern bool                       inputMode;

void plotPixel(int x, int y);
void bresenhamsLine(int x1, int y1, int x2, int y2);
void midpointLine(int x1, int y1, int x2, int y2);
void plotCirclePoints(int cx, int cy, int x, int y);
void bresenhamsCircle(int cx, int cy, int r);
void midpointCircle(int cx, int cy, int r);
void display();
void keyboard(unsigned char key, int x, int y);
void runMenu();

#endif