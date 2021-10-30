// Integer functions library

#include <cstdlib>

int digitsIn(int number) {
    int digits = 1;
    int absNumber = std::abs(number);
    while (absNumber /= 10) {
        ++digits;
    }
    return digits;
}
