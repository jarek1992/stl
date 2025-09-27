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

    // Sprawdzenie niedozwolonych znaków
    for (char c : input) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' &&
            c != '.' && c != '!' && c != ' ' && c != '%' && c != '^' && c != '$') {
            return ErrorCode::BadCharacter;
        }
    }

    // Niedozwolone przecinki
    if (input.find(',') != std::string::npos) {
        return ErrorCode::BadFormat;
    }

    std::istringstream iss(input);
    std::string token;
    double a = 0.0, b = 0.0;
    char op = 0;

    // Wczytanie pierwszej liczby wraz ze znakiem + lub -
    if (!(iss >> token))
        return ErrorCode::BadFormat;
    try {
        a = std::stod(token);
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // Wczytanie operatora
    if (!(iss >> op))
        return ErrorCode::BadFormat;
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    if (op == '!') {
        // factorial – po nim nie może być nic poza spacjami
        std::string rest;
        std::getline(iss, rest);
        for (char c : rest) {
            if (!std::isspace(c))
                return ErrorCode::BadFormat;
        }
        return operations.at(op)(a, 0.0, out);
    }

    // Wczytanie drugiej liczby wraz ze znakiem + lub -
    if (!(iss >> token))
        return ErrorCode::BadFormat;

    // Obsługa sytuacji typu "--77.321" lub "+32.1"
    if (token.size() > 1 && (token[0] == '+' || token[0] == '-') &&
        (token[1] == '+' || token[1] == '-')) {
        // podwójny operator, np. "++" lub "--"
        // przekształcamy na pojedynczy znak: -- -> +, ++ -> +
        int sign = 1;
        for (char c : token) {
            if (c == '-')
                sign *= -1;
        }
        try {
            b = std::stod(token.substr(token.find_first_of("0123456789.")));
            b *= sign;
        } catch (...) {
            return ErrorCode::BadFormat;
        }
    } else {
        try {
            b = std::stod(token);
        } catch (...) {
            return ErrorCode::BadFormat;
        }
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
