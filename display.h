#pragma once
#include "raster_algorithms.h"

#define WIN_W 1400
#define WIN_H  900

#define ORTHO_H 120.0f
#define ORTHO_W (ORTHO_H * ((float)WIN_W / (float)WIN_H))

extern int currentAlgo;

void renderText2D(float x, float y, const std::string& text);
void renderText(float x, float y, const char* text);
void displayAlgorithmSteps();
void drawGrid();
void drawPlottedCoordinates();
void display();
void keyboard(unsigned char key, int x, int y);
void runMenu();