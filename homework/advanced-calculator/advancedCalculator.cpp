#include <cmath>

#include "advancedCalculator.hpp"

std::string errorCodeToString(ErrorCode code) {
    switch (code) {
    case ErrorCode::Ok:
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
        return ErrorCode::Ok;
    };
    operations['-'] = [](double a, double b, double *out) { 
        *out = a - b;
        return ErrorCode::Ok; 
    };
    operations['*'] = [](double a, double b, double *out) { 
        *out = a * b;
        return ErrorCode::Ok; 
    };
    operations['/'] = [](double a, double b, double* out) {
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        }
        *out = a / b; 
        return ErrorCode::Ok;
    };
    operations['%'] = [](double a, double b, double* out) {
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        } else if (static_cast<long long>(a) != a || static_cast<long long>(b) != b) {
            return ErrorCode::ModuleOfNonIntegerValue;
        }
        *out = static_cast<long long>(a) % static_cast<long long>(b);
        return ErrorCode::Ok;
    };
    operations['^'] = [](double a, double b, double* out) { 
        *out = std::pow(a, b);
        return ErrorCode::Ok; 
        };
    operations['$'] = [](double a, double b, double* out) {
        if (b == 0.0) {
            return ErrorCode::DivideBy0;
        } else if (a < 0 && static_cast<long long>(b) % 2 == 0) {
            return ErrorCode::SqrtOfNegativeNumber;
        }
        *out = std::pow(a, 1.0 / b);
        return ErrorCode::Ok;
    };
    operations['!'] = [](double a, double, double* out) {
        *out = std::tgamma(a + 1);
        return ErrorCode::Ok;
    };
}

ErrorCode AdvancedCalculator::process(const std::string& input, double* out) const {
    auto it = operations.find(op);
    if (it != operations.end()) {
        return it->second(a, b);
    }
    return Result{ErrorCode::BadCharacter};
}


