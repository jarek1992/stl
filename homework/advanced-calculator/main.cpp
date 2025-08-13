#include <iostream>

#include "advancedCalculator.hpp"

int main() {
    AdvancedCalculator calc;
    double result = 0;

    if (calc.process("123 / 0", &result) == ErrorCode::DivideBy0) {
    
    }

    return 0;
}