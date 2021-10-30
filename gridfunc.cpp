// Grid functions library

#include <iomanip>
#include <iostream>
#include <string>
#include "intfunc.h"


void drawGrid(std::string** gridData,
              int gridXCount,
              int gridYCount,
              int gridXStartsAt,
              int gridYStartsAt) {
    
    // Declarations
    int leftMargin = digitsIn(gridYCount + gridYStartsAt);

    // Print left margin for number column
    std::cout << std::setfill(' ') << std::setw(leftMargin + 2) << " ";

    // Print top row
    std::cout << "#";
    for (int y = 0; y < gridYCount; ++y) {
        std::cout << "####";
    }
    std::cout << std::endl;

    // Print rows with values
    for (int y = gridYCount - 1; y >= 0; --y) {
        
        // Print number on left margin
        std::cout << " " << std::right << std::setfill(' ') << std::setw(leftMargin) << y + gridYStartsAt << " ";
        std::cout << "#";
        
        // Print values
        for (int x = 0; x < gridXCount - 1; ++x) {
            std::cout << " " << std::right << std::setfill(' ') << std::setw(1) << gridData[x][y] << " |";
        }
        std::cout << " " << std::right << std::setfill(' ') << std::setw(1) << gridData[gridXCount - 1][y] << " #";
        std::cout << std::endl;
        
        // Print left margin for number column
        std::cout << std::setfill(' ') << std::setw(leftMargin + 2) << " ";
        
        // Print bottom line
        std::cout << "#";
        if (y == 0) {
            for (int y = 0; y < gridYCount; ++y) {
                std::cout << "####";
            }
            std::cout << std::endl;
        } else {
            for (int x = 0; x < gridXCount - 1; ++x) {
                std::cout << "---+";
            }
            std::cout << "---#" << std::endl;
        }
    }

    // Print left margin for number column
    std::cout << std::setfill(' ') << std::setw(leftMargin + 3) << " ";

    // Print bottom numbers
    for (int i = 0; i < gridXCount; ++i) {
        std::cout << " " << std::right << std::setfill(' ') << std::setw(1) << i + gridXStartsAt << "  ";
    }
    std::cout << std::endl;

}

bool isSurroundingGrid(std::string content,
                               int x,
                               int y,
                               int maxX,
                               int maxY,
                               std::string** gridData) {
    
    // Documentation assumes the grid starts from top left for clarity
    
    // Check if top left cell is available and match content
    if (x > 0 && y > 0) {
        if (gridData[x - 1][y - 1] == content) {
            return true;
        }
    }
    
    // Check if top cell is available and match content
    if (y > 0) {
        if (gridData[x][y - 1] == content) {
            return true;
        }
    }
    
    // Check if top right cell is available and match content
    if (x < maxX && y > 0) {
        if (gridData[x + 1][y - 1] == content) {
            return true;
        }
    }
    
    // Check if left cell is available and match content
    if (x > 0) {
        if (gridData[x - 1][y] == content) {
            return true;
        }
    }
    
    // Check if right cell is available and match content
    if (x < maxX) {
        if (gridData[x + 1][y] == content) {
            return true;
        }
    }
    
    // Check if bottom left cell is available and match content
    if (x > 0 && y < maxY) {
        if (gridData[x - 1][y + 1] == content) {
            return true;
        }
    }
    
    // Check if bottom cell is available and match content
    if (y < maxY) {
        if (gridData[x][y + 1] == content) {
            return true;
        }
    }
    
    // Check if bottom right cell is available and match content
    if (x < maxX && y < maxY) {
        if (gridData[x + 1][y + 1] == content) {
            return true;
        }
    }
    
    return false;
    
}
