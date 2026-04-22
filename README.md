# Raster Drawing Algorithms in OpenGL (C++)

A modular implementation of classic raster drawing algorithms using OpenGL.

## Implemented Algorithms

1. **Bresenham Line Algorithm** - Efficient line drawing using integer arithmetic
2. **Midpoint Line Algorithm** - Alternative line drawing using decision parameter
3. **Bresenham Circle Algorithm** - Efficient circle drawing with 8-way symmetry
4. **Midpoint Circle Algorithm** - Alternative circle drawing using implicit equation

## Project Structure

```
├── main.cpp                  # Main entry point and OpenGL initialization
├── algorithms.cpp           # Implementation of all drawing algorithms
├── algorithms.h             # Function declarations for algorithms
├── display.cpp              # OpenGL display and keyboard callbacks
├── display.h                # Display function declarations
├── raster_algorithms.h      # Common constants and utility declarations
├── Makefile                 # Build configuration
└── README.md                # This file
```

## File Descriptions

### `raster_algorithms.h`
- Contains global constants (`WIN_W`, `WIN_H`)
- Function declarations for all algorithms
- Global `currentAlgo` variable for algorithm selection

### `algorithms.cpp`
- Implementation of `bresenhamsLine()` and `midpointLine()`
- Implementation of `bresenhamsCircle()` and `midpointCircle()`
- Helper function `plotPixel()` for rendering individual pixels
- Helper function `plotCirclePoints()` for 8-way symmetric plotting

### `display.cpp`
- OpenGL display callback implementation
- Keyboard callback for algorithm switching (keys 1-4, ESC to quit)
- Algorithm visualization with different colored shapes
- On-screen HUD with algorithm name and key hints

### `main.cpp`
- GLUT initialization and window creation
- Orthographic projection setup for 2D rendering
- Callback registration
- Console help message

## Installation & Execution Guide

### Prerequisites
- **MinGW** (C++ Compiler for Windows)
- **OpenGL** (usually comes with graphics drivers)
- **FreeGLUT** (OpenGL Utility Toolkit)
- **Make** (mingw32-make for building)

---

## Windows Installation

### Step 1: Install MinGW (if not installed)
```powershell
# Using Chocolatey (recommended)
choco install mingw

# Or download from: https://www.mingw-w64.org/
```

### Step 2: Install FreeGLUT
**Using MSYS2**
```powershell
# Install MSYS2 from https://www.msys2.org/
# Then in MSYS2 terminal:
pacman -S mingw-w64-x86_64-freeglut
```

## Building the Project

### Manual Compilation
```powershell
g++ main.cpp algorithms.cpp display.cpp -o raster.exe -lopengl32 -lglu32 -lfreeglut
```

---

## Running the Program

        ```powershell
# If you have raster.exe
.\raster.exe

```

---

## Usage Instructions

When you run the program, you'll see a menu:

```
==============================================
  Raster Drawing Algorithms
==============================================
  1. Bresenham Line Algorithm
  2. Midpoint Line Algorithm
  3. Bresenham Circle Algorithm
  4. Midpoint Circle Algorithm
  0. Exit
==============================================
  Select (0-4): 
```

**Simply press 1, 2, 3, or 4** to run each algorithm with predefined coordinates:
- **Option 1**: Bresenham Line from (-50, -30) to (70, 50)
- **Option 2**: Midpoint Line from (-60, 40) to (80, -20)
- **Option 3**: Bresenham Circle centered at (0, 0) with radius 60
- **Option 4**: Midpoint Circle centered at (20, -10) with radius 45

### OpenGL Window Controls
- **ESC**: Close window and return to menu
- **0**: Exit program

### Features in OpenGL Window
- **Grid**: Reference grid with coordinate axes
- **Algorithm Visualization**: Color-coded rendering (green/red for lines, magenta/cyan for circles)
- **Step Table**: For line algorithms (1 & 2), shows decision parameter values
- **Point List**: For circle algorithms (3 & 4), shows plotted coordinates
- **Algorithm Label**: Current algorithm name displayed at top

---
