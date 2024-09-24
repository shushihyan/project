#include <iostream>
#include <vector>
#include "complex.h"
#include "sorting.h"

int main() {
    std::vector<Complex> numbers = {
        Complex(3, 4),
        Complex(1, 1),
        Complex(0, 2),
        Complex(2, 2),
        Complex(5, 12)
    };

    for (const auto& number : numbers) {
        number.display();
        std::cout << "\n";
    }

    sortComplexNumbers(numbers);

    std::cout << "\n";
    for (const auto& number : numbers) {
        number.display();
        std::cout << "\n";
    }

    return 0;
}
