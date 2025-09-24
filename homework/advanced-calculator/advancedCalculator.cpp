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
    // 1. Usuń spacje
    std::string expr;
    for (char c : input) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            expr.push_back(c);
        }
    }
    if (expr.empty())
        return ErrorCode::BadFormat;

    // 2. Sprawdź znaki
    for (char c : expr) {
        if (!std::isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' &&
            c != '.' && c != '!' && c != '%' && c != '^' && c != '$') {
            return ErrorCode::BadCharacter;
        }
    }

    // 3. Szukaj operatora (pierwszy znak może być częścią liczby!)
    char op = 0;
    size_t opPos = std::string::npos;

    for (size_t i = 1; i < expr.size(); ++i) {  // zaczynamy od 1, żeby nie traktować '+'/'-' na początku jako operatora
        char c = expr[i];
        if (operations.count(c)) {
            if (op != 0) {
                // więcej niż jeden operator
                return ErrorCode::BadFormat;
            }
            op = c;
            opPos = i;
        }
    }

    if (op == 0)
        return ErrorCode::BadFormat;

    double a = 0, b = 0;

    try {
        if (op == '!') {
            // factorial: tylko lewa liczba
            a = std::stod(expr.substr(0, opPos));
        } else {
            // binary operator
            a = std::stod(expr.substr(0, opPos));
            b = std::stod(expr.substr(opPos + 1));
        }
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // 4. Wykonaj działanie
    return operations.at(op)(a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
