#include <fstream>
#include <iostream>
#include <string>
#include "strfunc.h"
#include "gridfunc.h"
#include "iofunc.h"
#include "weather.h"

// Displays the main menu and prompts the user for a valid choice
int mainMenu() {
    
    int selectedChoice;
    
    // Print top menu
    std::cout << "\033[2J\033[1;1H";
    std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
    std::cout << "Welcome to the Weather Information Processing System." << std::endl;
    std::cout << std::endl;
    std::cout << "1) Select and load configuration file" << std::endl;
    std::cout << "2) Display city map" << std::endl;
    std::cout << "3) Display cloud coverage map (cloudiness index)" << std::endl;
    std::cout << "4) Display cloud coverage map (LMH symbols)" << std::endl;
    std::cout << "5) Display atmospheric pressure map (pressure index)" << std::endl;
    std::cout << "6) Display atmospheric pressure map (LMH symbols)" << std::endl;
    std::cout << "7) Show weather forecast summary report" << std::endl;
    std::cout << "8) Quit" << std::endl;
    std::cout << std::endl;
    
    // Prompt user for choice and check the choice
    do {
        std::cout << "Please enter your choice: ";
        std::cin >> selectedChoice;
        std::cin.clear();
        if (selectedChoice > 0 && selectedChoice < 9) {
            return selectedChoice;
        } else {
            std::cout << "Invalid choice, please try again." << std::endl;
            selectedChoice = 0;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (!(selectedChoice > 0 && selectedChoice < 9));
    
    return selectedChoice;
    
}

// The main entry point of the program
int main() {
    
    // Main declarations
    std::string strConfigFilename,
                strCityLocationFilename,
                strCityCloudCoverFilename,
                strCityAtmosPressureFilename,
                strFileReaderLine,
                tempVar;
    int fileSequence,
        choice,
        gridXCount,
        gridYCount,
        cityCount;
    std::string* arrUniqueCityNames = nullptr;
    std::string* arrUniqueCityIDs = nullptr;
    std::string** arrCityName = nullptr;
    std::string** arrCityID = nullptr;
    std::string** arrCityCloudCoverShort = nullptr;
    std::string** arrCityCloudCoverSymb = nullptr;
    std::string** arrAtmosPressureShort = nullptr;
    std::string** arrAtmosPressureSymb = nullptr;
    int** arrCityCloudCover = nullptr;
    int** arrAtmosPressure = nullptr;
    unsigned gridXUpper,
             gridXLower,
             gridYUpper,
             gridYLower;
    bool isGoingToExit,
         isEnterKeyPressed;
    std::ifstream fileReader;
    
    isGoingToExit = false;
    
    do {
        
        choice = mainMenu();
        
        std::cout << "\033[2J\033[1;1H";
        
        switch (choice) {
        case 1:
            
            // Always cleanup before we load
            if (arrCityName) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrCityName[i];
                }
            }
            if (arrCityID) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrCityID[i];
                }
            }
            if (arrCityCloudCover) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrCityCloudCover[i];
                }
            }
            if (arrCityCloudCoverShort) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrCityCloudCoverShort[i];
                }
            }
            if (arrCityCloudCoverSymb) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrCityCloudCoverSymb[i];
                }
            }
            if (arrAtmosPressure) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrAtmosPressure[i];
                }
            }
            if (arrAtmosPressureShort) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrAtmosPressureShort[i];
                }
            }
            if (arrAtmosPressureSymb) {
                for (int i = 0; i < gridXCount; ++i) {
                    delete[] arrAtmosPressureSymb[i];
                }
            }
            
            delete[] arrCityName;
            delete[] arrCityID;
            delete[] arrCityCloudCover;
            delete[] arrCityCloudCoverShort;
            delete[] arrCityCloudCoverSymb;
            delete[] arrAtmosPressure;
            delete[] arrAtmosPressureShort;
            delete[] arrAtmosPressureSymb;
            
            // Prompt user for and store config filename
            std::cout << "Please enter the configuration file's name: ";
            std::cin >> strConfigFilename;
            
            std::cout << std::endl;
            std::cout << "Reading configuration... ";
            
            // Open config file with some intelligent error checking
            try {
                provideStream(strConfigFilename, fileReader);
            } catch (std::string e) {
                std::cerr << std::endl << e << std::endl;
                return 1;
            }
            
            // Start reading all the configuration data
            fileSequence = 0;
            cityCount = 0;
            while (fileReader.good()) {
                while (getline(fileReader, strFileReaderLine)) {
                    if (startsWith(strFileReaderLine, "//") || trimString(strFileReaderLine).length() == 0) {
                        // Ignore commented and blank lines
                    } else if (startsWith(strFileReaderLine, "GridX_IdxRange")) {
                        // Read in gridX upper and lower bounds
                        std::string s = getRightSide(strFileReaderLine, "=");
                        gridXLower = std::stoul(getLeftSide(s, "-"));
                        gridXUpper = std::stoul(getRightSide(s, "-"));
                    } else if (startsWith(strFileReaderLine, "GridY_IdxRange")) {
                        // Read in gridY upper and lower bounds
                        std::string s = getRightSide(strFileReaderLine, "=");
                        gridYLower = std::stoul(getLeftSide(s, "-"));
                        gridYUpper = std::stoul(getRightSide(s, "-"));
                    } else {
                        // Read in file names in sequential order
                        switch (fileSequence) {
                        case 0:
                            strCityLocationFilename = strFileReaderLine;
                            break;
                        case 1:
                            strCityCloudCoverFilename = strFileReaderLine;
                            break;
                        case 2:
                            strCityAtmosPressureFilename = strFileReaderLine;
                            break;
                        default: break;
                        }
                        ++fileSequence;
                    }
                }
            }
            
            // Error handling for if the hardware has failed
            if (fileReader.bad()) {
                std::cerr << "The configuration file could not be read correctly. A hardware device might have failed.";
                return 9;
            }
            
            // Close the configuration file
            fileReader.close();
            
            std::cout << "Done." << std::endl;
            
            // City map declarations
            gridXCount = gridXUpper - gridXLower + 1;
            gridYCount = gridYUpper - gridYLower + 1;
            
            // Initialize 2D arrays using grid X and Y size
            arrCityName = new std::string*[gridXCount];
            arrCityID = new std::string*[gridXCount];
            arrCityCloudCover = new int*[gridXCount];
            arrCityCloudCoverShort = new std::string*[gridXCount];
            arrCityCloudCoverSymb = new std::string*[gridXCount];
            arrAtmosPressure = new int*[gridXCount];
            arrAtmosPressureShort = new std::string*[gridXCount];
            arrAtmosPressureSymb = new std::string*[gridXCount];
            for (int i = 0; i < gridXCount; ++i) {
                arrCityName[i] = new std::string[gridYCount];
                arrCityID[i] = new std::string[gridYCount];
                arrCityCloudCover[i] = new int[gridYCount];
                arrCityCloudCoverShort[i] = new std::string[gridYCount];
                arrCityCloudCoverSymb[i] = new std::string[gridYCount];
                arrAtmosPressure[i] = new int[gridYCount];
                arrAtmosPressureShort[i] = new std::string[gridYCount];
                arrAtmosPressureSymb[i] = new std::string[gridYCount];
            }
            
            std::cout << "Reading city location information... ";
            
            // Open city location file
            try {
                provideStream(strCityLocationFilename, fileReader);
            } catch (std::string e) {
                std::cerr << std::endl << e << std::endl;
                return 1;
            }
            
            // Start reading city location data
            while (fileReader.good()) {
                while (getline(fileReader, strFileReaderLine)) {
                    if (startsWith(strFileReaderLine, "//")) {
                        // Ignore all lines with //
                    } else if (startsWith(strFileReaderLine, "[")) {
                        // Read in a grid item strings
                        std::string coord = getLeftSide(strFileReaderLine, "-");
                        std::string cityID = getLeftSide(getRightSide(strFileReaderLine, "-"), "-");
                        std::string cityName = getRightSide(getRightSide(strFileReaderLine, "-"), "-");
                        
                        // Process strings to coordinates
                        coord = coord.substr(1, coord.length() - 2);
                        int coordX = stoi(getLeftSide(coord, ","));
                        int coordY = stoi(trimString(getRightSide(coord, ",")));
                        
                        // Store the name and ID into the arrays
                        try {
                            arrCityName[coordX][coordY] = cityName;
                            arrCityID[coordX][coordY] = cityID;
                            
                            // Store the city into unique arrays of cities
                            if (arrUniqueCityNames && arrUniqueCityIDs) {
                                
                                // Check if city exists
                                bool isCityAlreadyExists = false;
                                for (int i = 0; i < cityCount; ++i) {
                                    if (arrUniqueCityNames[i] == cityName && arrUniqueCityIDs[i] == cityID) {
                                        isCityAlreadyExists = true;
                                        break;
                                    }
                                }
                                
                                // Add non-existent city to unique city array
                                if (!isCityAlreadyExists) {
                                    
                                    // Create an array with the old array's size + 1
                                    std::string* resizedUniqueCityNames = new std::string[cityCount + 1];
                                    std::string* resizedUniqueCityIDs = new std::string[cityCount + 1];
                                    
                                    // Copy the array data in
                                    for (int i = 0; i < cityCount; ++i) {
                                        resizedUniqueCityNames[i] = arrUniqueCityNames[i];
                                        resizedUniqueCityIDs[i] = arrUniqueCityIDs[i];
                                    }
                                    
                                    // Add the new city name and ID
                                    resizedUniqueCityNames[cityCount] = cityName;
                                    resizedUniqueCityIDs[cityCount] = cityID;
                                    
                                    // Throw away the old arrays
                                    delete[] arrUniqueCityNames;
                                    delete[] arrUniqueCityIDs;
                                    
                                    // Put in the brand new array
                                    arrUniqueCityNames = resizedUniqueCityNames;
                                    arrUniqueCityIDs = resizedUniqueCityIDs;
                                    
                                    ++cityCount;
                                }
                                
                            } else {
                                
                                ++cityCount;
                                
                                // Initialize the array if it's not initialized
                                arrUniqueCityNames = new std::string[cityCount];
                                arrUniqueCityIDs = new std::string[cityCount];
                                
                                // Set the first value of the array
                                arrUniqueCityNames[0] = cityName;
                                arrUniqueCityIDs[0] = cityID;
                                    
                            }
                            
                        } catch (std::exception& e) {
                            std::cerr << "Exception occurred while reading data: " << e.what() << std::endl;
                            std::cerr << "Line data: " << strFileReaderLine;
                            return 13;
                        }
                        
                        
                    }
                }
            }
            
            // Error handling for if the hardware has failed
            if (fileReader.bad()) {
                std::cerr << "The city location file could not be read correctly. A hardware device might have failed.";
                return 9;
            }
            
            // Close the city location file
            fileReader.close();
            
            std::cout << "Done." << std::endl;
            
            std::cout << "Reading cloud cover information... ";
            
            // Open cloud cover file
            try {
                provideStream(strCityCloudCoverFilename, fileReader);
            } catch (std::string e) {
                std::cerr << std::endl << e << std::endl;
                return 1;
            }
            
            // Start reading cloud cover data
            while (fileReader.good()) {
                while (getline(fileReader, strFileReaderLine)) {
                    if (startsWith(strFileReaderLine, "//")) {
                        // Ignore all lines with //
                    } else if (startsWith(strFileReaderLine, "[")) {
                        // Read in a grid item strings
                        std::string coord = getLeftSide(strFileReaderLine, "-");
                        int cloudCover = stoi(getRightSide(strFileReaderLine, "-"));
                        
                        // Process strings to coordinates
                        coord = coord.substr(1, coord.length() - 2);
                        int coordX = stoi(getLeftSide(coord, ","));
                        int coordY = stoi(trimString(getRightSide(coord, ",")));
                        
                        // Store the cloud cover information into the arrays
                        try {
                            arrCityCloudCover[coordX][coordY] = cloudCover;
                            arrCityCloudCoverShort[coordX][coordY] = std::to_string(cloudCover / 10);
                            arrCityCloudCoverSymb[coordX][coordY] = cloudCoverSymb(cloudCover);
                        } catch (std::exception& e) {
                            std::cerr << "Exception occurred while reading data: " << e.what() << std::endl;
                            std::cerr << "Line data: " << strFileReaderLine;
                            return 13;
                        }
                    }
                }
            }
            
            // Error handling for if the hardware has failed
            if (fileReader.bad()) {
                std::cerr << "The cloud cover information file could not be read correctly. A hardware device might have failed.";
                return 9;
            }
            
            // Close the cloud cover file
            fileReader.close();
            
            std::cout << "Done." << std::endl;
            
            std::cout << "Reading atmospheric pressure information... ";
            
            // Open atmospheric pressure file
            try {
                provideStream(strCityAtmosPressureFilename, fileReader);
            } catch (std::string e) {
                std::cerr << std::endl << e << std::endl;
                return 1;
            }
            
            // Start reading atmospheric pressure data
            while (fileReader.good()) {
                while (getline(fileReader, strFileReaderLine)) {
                    if (startsWith(strFileReaderLine, "//")) {
                        // Ignore all lines with //
                    } else if (startsWith(strFileReaderLine, "[")) {
                        // Read in a grid item strings
                        std::string coord = getLeftSide(strFileReaderLine, "-");
                        int atmosPressure = stoi(getRightSide(strFileReaderLine, "-"));
                        
                        // Process strings to coordinates
                        coord = coord.substr(1, coord.length() - 2);
                        int coordX = stoi(getLeftSide(coord, ","));
                        int coordY = stoi(trimString(getRightSide(coord, ",")));
                        
                        // Store the atmospheric pressure information into the arrays
                        try {
                            arrAtmosPressure[coordX][coordY] = atmosPressure;
                            arrAtmosPressureShort[coordX][coordY] = std::to_string(atmosPressure / 10);
                            arrAtmosPressureSymb[coordX][coordY] = atmosPressureSymb(atmosPressure);
                        } catch (std::exception& e) {
                            std::cerr << "Exception occurred while reading data: " << e.what() << std::endl;
                            std::cerr << "Line data: " << strFileReaderLine;
                            return 13;
                        }
                    }
                }
            }
            
            // Error handling for if the hardware has failed
            if (fileReader.bad()) {
                std::cerr << "The atmospheric pressure information file could not be read correctly. A hardware device might have failed.";
                return 9;
            }
            
            // Close the atmospheric pressure file
            fileReader.close();
            
            std::cout << "Done." << std::endl;
            
            std::cout << "Configuration file loaded successfully." << std::endl;
            
            break;
            
        case 2:
            
            std::cout << "City Map" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                drawGrid(arrCityID, gridXCount, gridYCount, gridXLower, gridYLower);
            }
            break;
            
        case 3:
            
            std::cout << "Cloud Coverage Map (Cloudiness Index)" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                drawGrid(arrCityCloudCoverShort, gridXCount, gridYCount, gridXLower, gridYLower);
            }
            break;
            
        case 4:
            
            std::cout << "Cloud Coverage Map (LMH Symbols)" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                drawGrid(arrCityCloudCoverSymb, gridXCount, gridYCount, gridXLower, gridYLower);
            }
            break;
            
        case 5:
            
            std::cout << "Atmospheric Pressure Map (Pressure Index)" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                drawGrid(arrAtmosPressureShort, gridXCount, gridYCount, gridXLower, gridYLower);
            }
            break;
            
        case 6:
            
            std::cout << "Atmospheric Pressure Map (LMH Symbols)" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                drawGrid(arrAtmosPressureSymb, gridXCount, gridYCount, gridXLower, gridYLower);
            }
            break;
            
        case 7:
            
            std::cout << "Weather Forecast Summary Report" << std::endl;
            std::cout << std::setfill('-') << std::setw(53) << "-" << std::endl;
            std::cout << std::endl;
            
            if (strConfigFilename == "") {
                std::cout << "Configuration file not loaded. Please load a configuration file before using this option." << std::endl;
            } else {
                for (int i = 0; i < cityCount; ++i) {
                    
                    double cloudCover = getCloudCover(arrUniqueCityIDs[i], arrCityID, arrCityCloudCover, gridXCount, gridYCount);
                    double atmosPressure = getAtmosPressure(arrUniqueCityIDs[i], arrCityID, arrAtmosPressure, gridXCount, gridYCount);
                    int rain = getRain(atmosPressureSymb((int)atmosPressure), cloudCoverSymb((int)cloudCover));
                    std::string weatherSymbol = getWeatherSymbol(rain);
                    
                    std::cout << "City Name: " << arrUniqueCityNames[i] << std::endl;
                    std::cout << "City ID  : " << arrUniqueCityIDs[i] << std::endl;
                    std::cout << "Ave. Cloud Cover (ACC)   : " << std::fixed << std::setprecision(2) << cloudCover << " (" << cloudCoverSymb((int)cloudCover) << ")" << std::endl;
                    std::cout << "Ave. Pressure (AP)       : " << std::fixed << std::setprecision(2) << atmosPressure << " (" << atmosPressureSymb((int)atmosPressure) << ")" << std::endl;
                    std::cout << "Probability of Rain (%)  : " << rain << ".00" << std::endl;
                    std::cout << weatherSymbol << std::endl << std::endl;
                    
                }
            }
            break;
            
        case 8:
            
            isGoingToExit = true;
            break;
            
        default: break;
        }
        
        if (0 < choice && choice < 8) {
            std::cout << std::endl << "Press Enter to return to the main menu.";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            isEnterKeyPressed = false;
            while (!isEnterKeyPressed) {
                if (std::cin.get() == '\n') {
                    isEnterKeyPressed = true;
                }
            }
        }
        
    } while (!isGoingToExit);
    
    std::cout << "Cleaning up before exiting... ";
    
    // Throw away anything we don't need anymore
    
    if (arrCityName) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrCityName[i];
        }
    }
    if (arrCityID) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrCityID[i];
        }
    }
    if (arrCityCloudCover) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrCityCloudCover[i];
        }
    }
    if (arrCityCloudCoverShort) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrCityCloudCoverShort[i];
        }
    }
    if (arrCityCloudCoverSymb) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrCityCloudCoverSymb[i];
        }
    }
    if (arrAtmosPressure) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrAtmosPressure[i];
        }
    }
    if (arrAtmosPressureShort) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrAtmosPressureShort[i];
        }
    }
    if (arrAtmosPressureSymb) {
        for (int i = 0; i < gridXCount; ++i) {
            delete[] arrAtmosPressureSymb[i];
        }
    }
    
    delete[] arrCityName;
    delete[] arrCityID;
    delete[] arrCityCloudCover;
    delete[] arrCityCloudCoverShort;
    delete[] arrCityCloudCoverSymb;
    delete[] arrAtmosPressure;
    delete[] arrAtmosPressureShort;
    delete[] arrAtmosPressureSymb;
    
    std::cout << "Done." << std::endl;
    
    std::cout << "Thank you for using the Weather Information Processing System." << std::endl;
    
    return 0;

}
