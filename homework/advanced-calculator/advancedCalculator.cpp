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
    if (!out)
        return ErrorCode::BadFormat;

    // Sprawdzenie niedozwolonych znaków i przecinków
    for (char c : input) {
        if (c == ',')
            return ErrorCode::BadFormat;
        if (!std::isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' &&
            c != '.' && c != '!' && c != ' ' && c != '%' && c != '^' && c != '$') {
            return ErrorCode::BadCharacter;
        }
    }

    std::istringstream iss(input);
    std::string token;
    double a = 0.0, b = 0.0;
    char op = 0;

    // Wczytanie pierwszej liczby (z możliwym znakiem + lub -)
    if (!(iss >> token))
        return ErrorCode::BadFormat;
    try {
        size_t pos;
        a = std::stod(token, &pos);
        if (pos != token.size())
            return ErrorCode::BadFormat;  // np. "12.4.3"
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // Wczytanie operatora
    if (!(iss >> op))
        return ErrorCode::BadFormat;
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    if (op == '!') {
        // Factorial – po nim nie może być nic oprócz spacji
        std::string rest;
        std::getline(iss, rest);
        for (char c : rest) {
            if (!std::isspace(c))
                return ErrorCode::BadFormat;
        }
        return operations.at(op)(a, 0.0, out);
    }

    // Wczytanie drugiej liczby (obsługa prefiksu + lub -)
    if (!(iss >> token))
        return ErrorCode::BadFormat;

    // Obsługa podwójnych znaków, np. "--77.321", "++12.3"
    int sign = 1;
    while (!token.empty() && (token[0] == '+' || token[0] == '-')) {
        if (token[0] == '-')
            sign *= -1;
        token.erase(0, 1);
    }

    if (token.empty())
        return ErrorCode::BadFormat;

    try {
        size_t pos;
        b = std::stod(token, &pos) * sign;
        if (pos != token.size())
            return ErrorCode::BadFormat;  // np. "12.4.3"
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // Sprawdzenie, czy po drugiej liczbie nie ma dodatkowych znaków
    std::string rest;
    std::getline(iss, rest);
    for (char c : rest) {
        if (!std::isspace(c))
            return ErrorCode::BadFormat;
    }

    return operations.at(op)(a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
