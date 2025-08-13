#include "advancedCalculator.hpp"

#include <cmath>

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
    operations['+'] = [](double a, double b) { return Result{ a + b }; };
    operations['-'] = [](double a, double b) { return Result{ a - b }; };
    operations['*'] = [](double a, double b) { return Result{ a * b }; };
    operations['/'] = [](double a, double b) {
        if (b == 0.0) {
            return Result{ErrorCode::DivideBy0};
        }
        return Result{a / b};
    };
    operations['%'] = [](double a, double b) {
        if (b == 0.0) {
            return Result{ErrorCode::DivideBy0};
        } else if (static_cast<long long>(a) != a || static_cast<long long>(b) != b) {
            return Result{ErrorCode::ModuleOfNonIntegerValue};
        }
        return Result{
            static_cast<long long>(a) % static_cast<long long>(b)};
    };
    operations['^'] = [](double a, double b) { return Result{std::pow(a, b)}; };
    operations['$'] = [](double a, double b) {
        if (b == 0.0) {
            return Result{ErrorCode::DivideBy0};
        } else if (a < 0 && static_cast<long long>(b) % 2 == 0) {
            return Result{ErrorCode::SqrtOfNegativeNumber};
        }
        return Result{
            std::pow(a, 1.0 / b)};
    };
    operations['!'] = [](double a, double) {
        return Result{
            std::tgamma(a + 1)};
    };
}

AdvancedCalculator::Result AdvancedCalculator::calculate(char op, double a, double b) const {
    auto it = operations.find(op);
    if (it != operations.end()) {
        return it->second(a, b);
    }
    return Result{ErrorCode::BadCharacter};
}


