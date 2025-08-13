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
    case ErrorCode::DividedBy0:
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
            return Result{ErrorCode::DividedBy0};
        }
        return Result{a / b};
    };
    operations['%'] = [](double a, double b) {
        if (static_cast<long long>(a) != a || static_cast<long long>(b) != b) {
            return Result{ErrorCode::ModuleOfNonIntegerValue};
        } else if (b == 0.0) {
            return Result{ErrorCode::DividedBy0};
        }
        return Result{
            static_cast<long long>(a) % static_cast<long long>(b)};
    };



        };