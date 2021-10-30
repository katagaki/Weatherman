#include <string>
#include "gridfunc.h"

#ifndef WEATHER
#define WEATHER
#endif

// Calculates the average cloud cover for a city using its ID
// locationData and cloudCoverData must contain a 2D array containing values for the calculation.
double getCloudCover(std::string cityID,
                     std::string** locationData,
                     int** cloudCoverData,
                     int gridXCount,
                     int gridYCount);

// Chooses the appropriate symbol for a certain amount of cloud cover
std::string cloudCoverSymb(int cloudCover);

// Calculates the average atmospheric pressure for a city using its ID
// locationData and atmosPressureData must contain a 2D array containing values for the calculation.
double getAtmosPressure(std::string cityID,
                        std::string** locationData,
                        int** atmosPressureData,
                        int gridXCount,
                        int gridYCount);

// Chooses the appropriate symbol for a certain amount of atmospheric pressure
std::string atmosPressureSymb(int atmosPressure);

// Caluclates the estimated amount of rain using atmospheric pressure and cloud cover symbols
int getRain(std::string atmosPressureSymb,
            std::string cloudCoverSymb);

// Chooses the appropriate ASCII art for the estimated amount of rain
std::string getWeatherSymbol(int rain);
