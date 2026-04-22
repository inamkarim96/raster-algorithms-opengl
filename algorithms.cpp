#include "raster_algorithms.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

// ── Globals ───────────────────────────────────────────────────────────────
vector<AlgorithmStep> algorithmSteps;
vector<PlottedPoint>  plottedPoints;
LineParams            lineParams   = {0, 0, 0, 0};
CircleParams          circleParams = {0, 0, 0};
int                   currentAlgo = 0;
bool                  inputMode   = false;

// ── Plot a single pixel ───────────────────────────────────────────────────
void plotPixel(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
    plottedPoints.push_back({x, y});
}

// ── 1. Bresenham Line ─────────────────────────────────────────────────────
void bresenhamsLine(int x1, int y1, int x2, int y2)
{
    algorithmSteps.clear();

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int x  = x1, y = y1;

    cout << "\n=== BRESENHAM LINE ALGORITHM ===" << endl;
    cout << "Line from (" << x1 << "," << y1 << ") to ("
         << x2 << "," << y2 << ")" << endl;
    cout << "dx=" << dx << ", dy=" << dy << endl;

    if (dx >= dy)
    {
        int p = 2 * dy - dx;
        cout << "Initial p = 2*dy - dx = " << p << endl;
        cout << "Step\t(x,y)\t\tp\t\tDecision" << endl;

        for (int i = 0; i <= dx; i++)
        {
            int cx = x, cy = y, cp = p;
            cout << i << "\t(" << cx << "," << cy << ")\t\t" << cp << "\t\t";
            plotPixel(x, y);

            ostringstream dec;
            if (p < 0)
            {
                cout << "p<0: p+=2*dy=" << (p+2*dy) << ", x++" << endl;
                dec << "p<0: p+=" << (2*dy) << "->" << (p+2*dy);
                p += 2 * dy;
            }
            else
            {
                cout << "p>=0: p+=2*(dy-dx)=" << (p+2*(dy-dx))
                     << ", x++, y++" << endl;
                dec << "p>=0: p+=" << (2*(dy-dx)) << "->" << (p+2*(dy-dx));
                p += 2 * (dy - dx);
                y += sy;
            }
            x += sx;
            algorithmSteps.push_back({i, cx, cy, cp, dec.str()});
        }
    }
    else
    {
        int p = 2 * dx - dy;
        cout << "Initial p = 2*dx - dy = " << p << endl;
        cout << "Step\t(x,y)\t\tp\t\tDecision" << endl;

        for (int i = 0; i <= dy; i++)
        {
            int cx = x, cy = y, cp = p;
            cout << i << "\t(" << cx << "," << cy << ")\t\t" << cp << "\t\t";
            plotPixel(x, y);

            ostringstream dec;
            if (p < 0)
            {
                cout << "p<0: p+=2*dx=" << (p+2*dx) << ", y++" << endl;
                dec << "p<0: p+=" << (2*dx) << "->" << (p+2*dx);
                p += 2 * dx;
            }
            else
            {
                cout << "p>=0: p+=2*(dx-dy)=" << (p+2*(dx-dy))
                     << ", x++, y++" << endl;
                dec << "p>=0: p+=" << (2*(dx-dy)) << "->" << (p+2*(dx-dy));
                p += 2 * (dx - dy);
                x += sx;
            }
            y += sy;
            algorithmSteps.push_back({i, cx, cy, cp, dec.str()});
        }
    }
    cout << "=== END ===" << endl;
}

// ── 2. Midpoint Line ──────────────────────────────────────────────────────
void midpointLine(int x1, int y1, int x2, int y2)
{
    algorithmSteps.clear();

    int dx = x2 - x1;
    int dy = y2 - y1;
    int stepNum = 0;

    cout << "\n=== MIDPOINT LINE ALGORITHM ===" << endl;
    cout << "Line from (" << x1 << "," << y1 << ") to ("
         << x2 << "," << y2 << ")" << endl;
    cout << "dx=" << dx << ", dy=" << dy << endl;
    cout << "Step\t(x,y)\t\td\t\tDecision" << endl;

    if (dy <= dx)
    {
        int d = dy - (dx / 2);
        int x = x1, y = y1;
        cout << "Initial d = dy - dx/2 = " << d << endl;

        cout << stepNum << "\t(" << x << "," << y << ")\t\t"
             << d << "\t\t[initial]" << endl;
        algorithmSteps.push_back({stepNum++, x, y, d, "initial"});
        plotPixel(x, y);

        while (x < x2)
        {
            x++;
            int cd = d;
            ostringstream dec;
            if (d < 0)
            {
                d += dy;
                cout << stepNum << "\t(" << x << "," << y << ")\t\t"
                     << cd << "\t\td<0: E, d+=" << dy << "->" << d << endl;
                dec << "d<0: E, d+=" << dy << "->" << d;
            }
            else
            {
                d += dy - dx;
                y++;
                cout << stepNum << "\t(" << x << "," << y << ")\t\t"
                     << cd << "\t\td>=0: NE, d+=(dy-dx)="
                     << (dy-dx) << "->" << d << endl;
                dec << "d>=0: NE, d+=" << (dy-dx) << "->" << d;
            }
            algorithmSteps.push_back({stepNum++, x, y, cd, dec.str()});
            plotPixel(x, y);
        }
    }
    else
    {
        int d = dx - (dy / 2);
        int x = x1, y = y1;
        cout << "Initial d = dx - dy/2 = " << d << endl;

        cout << stepNum << "\t(" << x << "," << y << ")\t\t"
             << d << "\t\t[initial]" << endl;
        algorithmSteps.push_back({stepNum++, x, y, d, "initial"});
        plotPixel(x, y);

        while (y < y2)
        {
            y++;
            int cd = d;
            ostringstream dec;
            if (d < 0)
            {
                d += dx;
                cout << stepNum << "\t(" << x << "," << y << ")\t\t"
                     << cd << "\t\td<0: E, d+=" << dx << "->" << d << endl;
                dec << "d<0: E, d+=" << dx << "->" << d;
            }
            else
            {
                d += dx - dy;
                x++;
                cout << stepNum << "\t(" << x << "," << y << ")\t\t"
                     << cd << "\t\td>=0: NE, d+=(dx-dy)="
                     << (dx-dy) << "->" << d << endl;
                dec << "d>=0: NE, d+=" << (dx-dy) << "->" << d;
            }
            algorithmSteps.push_back({stepNum++, x, y, cd, dec.str()});
            plotPixel(x, y);
        }
    }
    cout << "=== END ===" << endl;
}

// ── Circle helper ─────────────────────────────────────────────────────────
void plotCirclePoints(int cx, int cy, int x, int y)
{
    plotPixel(cx+x, cy+y); plotPixel(cx-x, cy+y);
    plotPixel(cx+x, cy-y); plotPixel(cx-x, cy-y);
    plotPixel(cx+y, cy+x); plotPixel(cx-y, cy+x);
    plotPixel(cx+y, cy-x); plotPixel(cx-y, cy-x);
}

// ── 3. Bresenham Circle ───────────────────────────────────────────────────
void bresenhamsCircle(int cx, int cy, int r)
{
    int x = 0, y = r;
    int d = 3 - 2 * r;

    cout << "\n=== BRESENHAM CIRCLE ALGORITHM ===" << endl;
    cout << "Center (" << cx << "," << cy << ") radius " << r << endl;
    cout << "Initial d = 3 - 2*r = " << d << endl;
    cout << "Step\t(x,y)\t\td\t\tDecision" << endl;

    int step = 0;
    while (x <= y)
    {
        int cd = d;
        cout << step << "\t(" << x << "," << y << ")\t\t" << cd << "\t\t";
        plotCirclePoints(cx, cy, x, y);

        if (d < 0)
        {
            cout << "d<0: d+=4*x+6=" << (d+4*x+6) << endl;
            d += 4 * x + 6;
        }
        else
        {
            cout << "d>=0: d+=4*(x-y)+10=" << (d+4*(x-y)+10) << endl;
            d += 4 * (x - y) + 10;
            y--;
        }
        x++; step++;
    }
    cout << "=== END ===" << endl;
}

// ── 4. Midpoint Circle ────────────────────────────────────────────────────
void midpointCircle(int cx, int cy, int r)
{
    int x = 0, y = r;
    int p = 1 - r;

    cout << "\n=== MIDPOINT CIRCLE ALGORITHM ===" << endl;
    cout << "Center (" << cx << "," << cy << ") radius " << r << endl;
    cout << "Initial p = 1 - r = " << p << endl;
    cout << "Step\t(x,y)\t\tp\t\tDecision" << endl;

    int step = 0;
    while (x <= y)
    {
        int cp = p;
        cout << step << "\t(" << x << "," << y << ")\t\t" << cp << "\t\t";
        plotCirclePoints(cx, cy, x, y);

        if (p < 0)
        {
            cout << "p<0: p+=2*x+3=" << (p+2*x+3) << endl;
            p += 2 * x + 3;
        }
        else
        {
            cout << "p>=0: p+=2*(x-y)+5=" << (p+2*(x-y)+5) << endl;
            p += 2 * (x - y) + 5;
            y--;
        }
        x++; step++;
    }
    cout << "=== END ===" << endl;
}