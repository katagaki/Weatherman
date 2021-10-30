// Weather calculator library

#include <string>
#include "gridfunc.h"

double getCloudCover(std::string cityID,
                     std::string** locationData,
                     int** cloudCoverData,
                     int gridXCount = 0,
                     int gridYCount = 0) {
    
    int cellsCovered = 0;
    int maxX = gridXCount - 1;
    int maxY = gridYCount - 1;
    int totalCloudCover = 0;
        
    for (int x = 0; x < gridXCount; ++x) {
        for (int y = 0; y < gridYCount; ++y) {
            if (locationData[x][y] == cityID) {
                ++cellsCovered;
                totalCloudCover += cloudCoverData[x][y];
            } else {
                if (isSurroundingGrid(cityID, x, y, maxX, maxY, locationData)) {
                    ++cellsCovered;
                    totalCloudCover += cloudCoverData[x][y];
                }
            }
        }
    }
    
    return (1.0 * totalCloudCover) / cellsCovered;
    
}

std::string cloudCoverSymb(int cloudCover) {
    if (0 <= cloudCover && cloudCover < 35 ) {
        return "L";
    } else if (35 <= cloudCover && cloudCover < 65) {
        return "M";
    } else if (65 <= cloudCover && cloudCover < 100) {
        return "H";
    } else {
        return " ";
    }
}

double getAtmosPressure(std::string cityID,
                        std::string** locationData,
                        int** atmosPressureData,
                        int gridXCount,
                        int gridYCount) {
    
    int cellsCovered = 0;
    int maxX = gridXCount - 1;
    int maxY = gridYCount - 1;
    int totalAtmosPressure = 0;
        
    for (int x = 0; x < gridXCount; ++x) {
        for (int y = 0; y < gridYCount; ++y) {
            if (locationData[x][y] == cityID) {
                ++cellsCovered;
                totalAtmosPressure += atmosPressureData[x][y];
            } else {
                if (isSurroundingGrid(cityID, x, y, maxX, maxY, locationData)) {
                    ++cellsCovered;
                    totalAtmosPressure += atmosPressureData[x][y];
                }
            }
        }
    }
    
    return (1.0 * totalAtmosPressure) / cellsCovered;
    
}

std::string atmosPressureSymb(int atmosPressure) {
    if (0 <= atmosPressure && atmosPressure < 35 ) {
        return "L";
    } else if (35 <= atmosPressure && atmosPressure < 65) {
        return "M";
    } else if (65 <= atmosPressure && atmosPressure < 100) {
        return "H";
    } else {
        return " ";
    }
}

int getRain(std::string atmosPressureSymb,
            std::string cloudCoverSymb) {
    if (atmosPressureSymb == "L") {
        if (cloudCoverSymb == "H") {
            return 90;
        } else if (cloudCoverSymb == "M") {
            return 80;
        } else if (cloudCoverSymb == "L") {
            return 70;
        }
    } else if (atmosPressureSymb == "M") {
        if (cloudCoverSymb == "H") {
            return 60;
        } else if (cloudCoverSymb == "M") {
            return 50;
        } else if (cloudCoverSymb == "L") {
            return 40;
        }
    } else if (atmosPressureSymb == "H") {
        if (cloudCoverSymb == "H") {
            return 30;
        } else if (cloudCoverSymb == "M") {
            return 20;
        } else if (cloudCoverSymb == "L") {
            return 10;
        }
    }
    return 0;
}

std::string getWeatherSymbol(int rain) {
    switch (rain) {
        case 90: return "~~~\n~~~~~\n\\\\\\\\\\";
        case 80: return "~~~~\n~~~~~\n \\\\\\\\";
        case 70: return "~~~~\n~~~~~\n  \\\\\\";
        case 60: return "~~~~\n~~~~~\n   \\\\";
        case 50: return "~~~~\n~~~~~\n    \\";
        case 40: return "~~~~\n~~~~~\n";
        case 30: return "~~~\n~~~~\n";
        case 20: return "~~\n~~~\n";
        case 10: return "~\n~~\n";
        default: return "\n\n\n";
    }
}
