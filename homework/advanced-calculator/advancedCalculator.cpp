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

    // 1. Sprawdzenie niedozwolonych znaków
    for (char c : input) {
        if (!(std::isdigit(c) || c == '.' || std::isspace(c) || operations.count(c))) {
            return ErrorCode::BadCharacter;
        }
    }

    // 2. Sprawdzenie przecinka
    if (input.find(',') != std::string::npos)
        return ErrorCode::BadFormat;

    std::istringstream iss(input);
    double a = 0.0, b = 0.0;
    char op = 0;

    // 3. Wczytanie pierwszej liczby
    if (!(iss >> a))
        return ErrorCode::BadFormat;

    // 4. Wczytanie operatora
    if (!(iss >> op)) {
        // tylko jedna liczba → OK (np. "42")
        *out = a;
        return ErrorCode::OK;
    }

    // 5. Sprawdzenie, czy operator jest znany
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    // 6. Operator unarny !
    if (op == '!') {
        std::string leftover;
        if (iss >> leftover)
            return ErrorCode::BadFormat;  // np. "5! 2"
        return operations['!'](a, 0, out);
    }

    // 7. Operator binarny
    if (!(iss >> b))
        return ErrorCode::BadFormat;  // np. "+ 2"

    // 8. Sprawdzenie dodatkowych znaków po drugiej liczbie
    std::string rest;
    std::getline(iss, rest);
    for (char c : rest) {
        if (!std::isspace(c))
            return ErrorCode::BadFormat;
    }

    // 9. Wywołanie odpowiedniej operacji
    return operations[op](a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
