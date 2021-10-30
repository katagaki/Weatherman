// IO functions library

#include <fstream>
#include <iostream>

void provideStream(std::string filename, std::ifstream& fileReader) {
    fileReader.open(filename.c_str());
    if (fileReader.good()) {
        if (!fileReader) {
            throw std::string("The file '" + filename + "' could not be opened. Please check that the file is available and not locked by another process.");
        }
    } else {
        throw std::string("The file '" + filename + "' could not be found. Please ensure that the file name is correct, then try again.");
    }
}
