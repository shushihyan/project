#include "sorting.h"
#include <algorithm>

bool compareAbsolute(const Complex& a, const Complex& b) {
    return a.absolute() < b.absolute();
}

void sortComplexNumbers(std::vector<Complex>& numbers) {
    std::sort(numbers.begin(), numbers.end(), compareAbsolute);
}
