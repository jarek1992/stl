#include <cmath>
#include <sstream>

#include "advancedCalculator.hpp"

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
        bool negative = a < 0;
        a = std::fabs(a);

        if (a != std::floor(a)) {
            return ErrorCode::BadFormat;
        }

        double result = 1;
        for (int i = 1; i <= static_cast<int>(a); ++i) {
            result *= i;
        }
      
        *out = negative ? -result : result;
        return ErrorCode::OK;
    };
}

ErrorCode AdvancedCalculator::process(const std::string& input, double* out)  {
    std::istringstream iss(input);
    double a = 0;
    double b = 0;
    char op = 0;

    iss >> a >> op;
    if (!iss || operations.find(op) == operations.end()) {
        return ErrorCode::BadCharacter;
    }

    if (op != '!') {
        iss >> b;
        if (!iss) {
            return ErrorCode::BadFormat;
        }
    }

    char extra;
    if (iss >> extra) {
        return ErrorCode::BadCharacter;
    }

    return operations.at(op)(a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}