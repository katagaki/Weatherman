// String functions library

#include <string>

bool startsWith(std::string s, std::string keyword) {
    return s.find(keyword) == 0;
}

std::string trimString(std::string s) {
    if (s.length() == 0) {
        return s;
    } else {
        if (s.find(" ") == 0 && s.rfind(" ") == s.length() - 1) {
            return trimString(s.substr(1, s.length() - 2));
        } else if (s.find(" ") == 0 && s.rfind(" ") == std::string::npos) {
            return trimString(s.substr(1, s.length() - 1));
        } else if (s.find(" ") == std::string::npos && s.rfind(" ") == s.length() - 1) {
            return trimString(s.substr(0, s.length() - 2));
        } else {
            return s;
        }
    }
}

std::string removeLastChar(std::string s) {
    return s.substr(0, s.length() - 2);
}

std::string getLeftSide(std::string s, std::string delimiter) {
    int delimiterPos = s.find(delimiter);
    return s.substr(0, delimiterPos);
}

std::string getRightSide(std::string s, std::string delimiter) {
    int delimiterPos = s.find(delimiter);
    return s.substr(delimiterPos + 1);
}
