#include "raster_algorithms.h"

int main(int argc, char** argv)
{
    // glutInit must be called once before any GL work
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Start terminal menu — GL window opens only after input is complete
    runMenu();

    return 0;
}