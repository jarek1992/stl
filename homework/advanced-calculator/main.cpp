#include <iostream>

#include "advancedCalculator.hpp"

int main() {
    AdvancedCalculator calc;
    std::string input;
    double result = 0;

    std::cout << "Advanced calculator (quit or exit to close the program)\n";
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit" || input == "quit") {
            break;
        }

        ErrorCode code = calc.process(input, &result);
        if (code == ErrorCode::OK) {
            std::cout << "resut: " << result << "\n";
        } else {
            std::cout << errorCodeToString(code) << "\n";
        }
    }
    return 0;
}