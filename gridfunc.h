#include <iomanip>
#include <iostream>
#include <string>
#include "intfunc.h"

#ifndef GRIDFUNC
#define GRIDFUNC
#endif

// Draws a grid based on grid data provided
// Grid data must only contain 1 character per grid cell
void drawGrid(std::string** gridData,
              int gridXCount,
              int gridYCount,
              int gridXStartsAt,
              int gridYStartsAt);

// Checks if a grid cell is the surrounding cell for another grid cell with a certain value
bool isSurroundingGrid(std::string content,
                       int x,
                       int y,
                       int maxX,
                       int maxY,
                       std::string** gridData);
