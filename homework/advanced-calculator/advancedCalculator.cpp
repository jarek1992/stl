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

    std::string str;
    for (char c : input) {
        if (!std::isspace(c))
            str += c;  // usuń wszystkie spacje
    }

    if (str.empty())
        return ErrorCode::BadFormat;

    // sprawdzenie niepoprawnych znaków
    for (char c : str) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != '*' &&
            c != '/' && c != '.' && c != '!' &&
            c != '%' && c != '^' && c != '$') {
            if (c == ',')
                return ErrorCode::BadFormat;
            return ErrorCode::BadCharacter;
        }
    }

    // factorial specjalny przypadek
    if (str.back() == '!') {
        std::string left = str.substr(0, str.size() - 1);
        if (left.empty())
            return ErrorCode::BadFormat;

        char* endptr = nullptr;
        double a = std::strtod(left.c_str(), &endptr);
        if (*endptr != '\0')
            return ErrorCode::BadFormat;

        return operations.at('!')(a, 0.0, out);
    }

    // szukamy operatora (poza znakiem pierwszej liczby)
    size_t pos = 0;
    if (str[0] == '+' || str[0] == '-')
        pos = 1;

    size_t opPos = std::string::npos;
    char op = 0;
    for (; pos < str.size(); ++pos) {
        if (operations.find(str[pos]) != operations.end()) {
            opPos = pos;
            op = str[pos];
            break;
        }
    }

    if (opPos == std::string::npos)
        return ErrorCode::BadFormat;

    std::string left = str.substr(0, opPos);
    std::string right = str.substr(opPos + 1);

    if (left.empty() || right.empty())
        return ErrorCode::BadFormat;

    char* endptr = nullptr;
    double a = std::strtod(left.c_str(), &endptr);
    if (*endptr != '\0')
        return ErrorCode::BadFormat;

    double b = 0.0;
    b = std::strtod(right.c_str(), &endptr);
    if (*endptr != '\0')
        return ErrorCode::BadFormat;

    return operations.at(op)(a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
