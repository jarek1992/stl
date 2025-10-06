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
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        }
        *out = a / b;
        return ErrorCode::OK;
    };
    operations['%'] = [](double a, double b, double* out) {
        if (std::floor(a) != a || std::floor(b) != b) {
            return ErrorCode::ModuleOfNonIntegerValue;
        } else if (b == 0.0) {
            return ErrorCode::DivideBy0;
        }
        *out = static_cast<long long>(a) % static_cast<long long>(b);
        return ErrorCode::OK;
    };
    operations['^'] = [](double a, double b, double* out) {
        *out = std::pow(a, b);
        return ErrorCode::OK;
    };
    operations['$'] = [](double a, double b, double* out) {
        if (a < 0) {
            return ErrorCode::SqrtOfNegativeNumber;
        }
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        } else if (a < 0 && static_cast<long long>(b) % 2 == 0) {
            return ErrorCode::SqrtOfNegativeNumber;
        }
        *out = std::pow(a, 1.0 / b);
        return ErrorCode::OK;
    };
    operations['!'] = [](double a, double, double* out) {
        if (std::isnan(a) || std::isinf(a)) {
            return ErrorCode::BadFormat;
        }

        long double result = 1.0;
        double x = std::fabs(a);

        if (x == std::floor(x)) {
            for (int i = 1; i <= static_cast<int>(x); ++i) {
                result *= i;
            }
        } else {
            result = tgamma(x + 1);
        }

        *out = (a < 0 ? -result : result);
        return ErrorCode::OK;
    };
}

ErrorCode AdvancedCalculator::process(const std::string& input, double* out) {
    if (input.empty())
        return ErrorCode::BadFormat;

    // 1. Sprawdzenie niedozwolonych znaków (tylko znaki niebędące cyframi, kropką, spacją lub operatorem)
    for (char c : input) {
        if (!(std::isdigit(c) || std::isspace(c) || c == '.' || operations.count(c) || c == '-' || c == '+')) {
            return ErrorCode::BadCharacter;
        }
    }

    std::istringstream iss(input);
    double a = 0.0, b = 0.0;
    char op = 0;

    // 2. Wczytanie pierwszej liczby
    std::string tokenA;
    if (!(iss >> tokenA))
        return ErrorCode::BadFormat;

    // Sprawdzenie liczby pod kątem wielu kropek
    int dotCount = 0;
    for (size_t i = 0; i < tokenA.size(); ++i) {
        if (tokenA[i] == '.')
            dotCount++;
        if (dotCount > 1)
            return ErrorCode::BadFormat;
        if (!std::isdigit(tokenA[i]) && !(i == 0 && (tokenA[i] == '-' || tokenA[i] == '+')) && tokenA[i] != '.') {
            return ErrorCode::BadFormat;
        }
    }

    // Zamiana string na double
    try {
        a = std::stod(tokenA);
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // 3. Wczytanie operatora
    if (!(iss >> op)) {
        *out = a;  // tylko jedna liczba → OK
        return ErrorCode::OK;
    }

    // Nieznany operator
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    // Operator unarny !
    if (op == '!') {
        std::string leftover;
        if (iss >> leftover)
            return ErrorCode::BadFormat;  // np. "5! 2"
        return operations['!'](a, 0, out);
    }

    // 4. Wczytanie drugiej liczby
    std::string tokenB;
    if (!(iss >> tokenB))
        return ErrorCode::BadFormat;

    // Sprawdzenie drugiej liczby pod kątem wielu kropek
    dotCount = 0;
    for (size_t i = 0; i < tokenB.size(); ++i) {
        if (tokenB[i] == '.')
            dotCount++;
        if (dotCount > 1)
            return ErrorCode::BadFormat;
        if (!std::isdigit(tokenB[i]) && !(i == 0 && (tokenB[i] == '-' || tokenB[i] == '+')) && tokenB[i] != '.') {
            return ErrorCode::BadFormat;
        }
    }

    try {
        b = std::stod(tokenB);
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // 5. Sprawdzenie dodatkowych znaków po drugiej liczbie
    std::string rest;
    if (iss >> rest)
        return ErrorCode::BadFormat;

    // 6. Obliczenie wyniku
    return operations[op](a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
