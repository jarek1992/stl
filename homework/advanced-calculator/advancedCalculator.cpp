#include "advancedCalculator.hpp"
#include <cmath>
#include <sstream>

std::string errorCodeToString(ErrorCode code) {
    switch (code) {
    case ErrorCode::OK:
        return "Brak błędu.";
    case ErrorCode::BadCharacter:
        return "Nieprawidłowy znak — dozwolone są tylko liczby.";
    case ErrorCode::BadFormat:
        return "Zły format komendy.";
    case ErrorCode::DivideBy0:
        return "Błąd: dzielenie przez zero.";
    case ErrorCode::SqrtOfNegativeNumber:
        return "Błąd: pierwiastek z liczby ujemnej.";
    case ErrorCode::ModuleOfNonIntegerValue:
        return "Błąd: operacja % wymaga liczb całkowitych.";
    default:
        return "Nieznany błąd.";
    }
}

AdvancedCalculator::AdvancedCalculator() {
    operations['+'] = [](double a, double b, double* out) {
        *out = a + b;
        return ErrorCode::OK;
    };
    operations['-'] = [](double a, double b, double* out) {
        *out = a - b;
        return ErrorCode::OK;
    };
    operations['*'] = [](double a, double b, double* out) {
        *out = a * b;
        return ErrorCode::OK;
    };
    operations['/'] = [](double a, double b, double* out) {
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        *out = a / b;
        return ErrorCode::OK;
    };
    operations['%'] = [](double a, double b, double* out) {
        if (std::floor(a) != a || std::floor(b) != b)
            return ErrorCode::ModuleOfNonIntegerValue;
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        *out = static_cast<long long>(a) % static_cast<long long>(b);
        return ErrorCode::OK;
    };
    operations['^'] = [](double a, double b, double* out) {
        *out = std::pow(a, b);
        return ErrorCode::OK;
    };
    operations['$'] = [](double a, double b, double* out) {
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        if (a < 0 && std::floor(b) == b && static_cast<int>(b) % 2 == 0)
            return ErrorCode::SqrtOfNegativeNumber;
        *out = std::pow(a, 1.0 / b);
        return ErrorCode::OK;
    };
    operations['!'] = [](double a, double, double* out) {
        if (std::floor(a) == a) {
            long double result = 1;
            for (int i = 1; i <= static_cast<int>(std::abs(a)); ++i) {
                result *= i;
            }
            *out = std::copysign(result, a);
            return ErrorCode::OK;
        } else {
            double result = std::tgamma(a + 1);
            *out = std::copysign(result, a);
            return ErrorCode::OK;
        }
    };
}

ErrorCode AdvancedCalculator::process(const std::string& input, double* out) {
    if (input.empty()) {
        return ErrorCode::BadFormat;
    }

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        bool const is_valid_character = std::isdigit(c) ||
                                        std::isspace(c) ||
                                        c == ',' || c == '.' ||
                                        (operations.count(c) > 0);
        std::isspace(c) ||
            c == ',' || c == '.' ||
            (operations.count(c) > 0);

        if (!is_valid_character) {
            return ErrorCode::BadCharacter;
        }
    }

    int dotCount = 0;
    // 2. Obsłuż złe formatowanie kropek i przecinków
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        // 2. Obsłuż przecinek
        if (c == ',') {
            // 1a. Bad Formatting  - jeśli przecinek użyty zamiast kropki
            bool beforeDigit = (i > 0 && std::isdigit(input[i - 1]));
            bool afterDigit = (i + 1 < input.size() && std::isdigit(input[i + 1]));
            if (beforeDigit && afterDigit)
                return ErrorCode::BadFormat;  // np. "5,1!"
            // 1b. W przeciwnym przypadku bad character
            return ErrorCode::BadCharacter;
        } else if (c == '.') {
            dotCount++;
            if (dotCount > 1) {
                return ErrorCode::BadFormat;  // np. "12.4.3"
            }
        } else if (std::isspace(c) || (operations.count(c) > 0)) {
            dotCount = 0;
        }
    }

    for (size_t i = 0; i < input.size(); ++i) {
        // Skip until meaningful input
        if (std::isspace(input[i]))
            continue;
        // Correct input
        if (std::isdigit(input[i]) || input[i] == '-')
            break;
        return ErrorCode::BadFormat;
    }

    // --- parser ---
    std::istringstream iss(input);
    double a = 0.0, b = 0.0;
    char op = 0;

    // pierwsza liczba
    if (!(iss >> a))
        return ErrorCode::BadFormat;

    // operator
    if (!(iss >> op)) {
        *out = a;
        return ErrorCode::OK;
    }

    // nieznany operator
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    // silnia
    if (op == '!') {
        std::string extra;
        if (iss >> extra)
            return ErrorCode::BadFormat;
    }
    return operations['!'](a, 0, out);

    // druga liczba
    if (!(iss >> b))
        return ErrorCode::BadFormat;

    // po drugiej liczbie nie może być nic więcej
    std::string rest;
    if (iss >> rest)
        return ErrorCode::BadFormat;

    // pierwiastkowanie ujemnych liczb
    if (op == '$') {
        if (a < 0) {
            return ErrorCode::SqrtOfNegativeNumber;
        }
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        }
    }

    // obliczenie wyniku
    return operations[op](a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
