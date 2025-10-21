#include "advancedCalculator.hpp"
#include <algorithm>
#include <cctype>
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
    operations['+'] = [](double a, double b, double* out) { *out = a + b; return ErrorCode::OK; };
    operations['-'] = [](double a, double b, double* out) { *out = a - b; return ErrorCode::OK; };
    operations['*'] = [](double a, double b, double* out) { *out = a * b; return ErrorCode::OK; };
    operations['/'] = [](double a, double b, double* out) {
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        *out = a / b;
        return ErrorCode::OK;
    };
    operations['%'] = [](double a, double b, double* out) {
        if (std::floor(a) != a || std::floor(b) != b)
            return ErrorCode::ModuleOfNonIntegerValue;
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        *out = static_cast<long long>(a) % static_cast<long long>(b);
        return ErrorCode::OK;
    };
    operations['^'] = [](double a, double b, double* out) {
        *out = std::pow(a, b);
        return ErrorCode::OK;
    };
    operations['$'] = [](double a, double b, double* out) {
        if (b == 0.0)
            return ErrorCode::DivideBy0;
        if (a < 0 && std::fmod(b, 2) == 0)
            return ErrorCode::SqrtOfNegativeNumber;
        *out = std::pow(a, 1.0 / b);
        return ErrorCode::OK;
    };
    operations['!'] = [](double a, double, double* out) {
        if (a < 0.0)
            return ErrorCode::BadFormat;
        if (a == 0.0 || a == 1.0) {
            *out = 1;
            return ErrorCode::OK;
        }
        if (a != std::floor(a)) {
            *out = std::tgamma(a + 1);
            return ErrorCode::OK;
        }
        long double result = 1;
        for (int i = 1; i <= static_cast<int>(a); ++i)
            result *= i;
        *out = result;
        return ErrorCode::OK;
    };
}

ErrorCode AdvancedCalculator::process(const std::string& input, double* out) {
    if (input.empty())
        return ErrorCode::BadFormat;

    // wykrycie nieprawidłowych znaków lub przecinków
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == ',') {
            bool beforeDigit = (i > 0 && std::isdigit(input[i - 1]));
            bool afterDigit = (i + 1 < input.size() && std::isdigit(input[i + 1]));
            if (beforeDigit && afterDigit)
                return ErrorCode::BadFormat;  // np. "5,1"
            else
                return ErrorCode::BadCharacter;  // np. "5, + 2"
        }
        if (!(std::isdigit(c) || std::isspace(c) || c == '.' || operations.count(c))) {
            return ErrorCode::BadCharacter;
        }
    }

    std::istringstream iss(input);
    double a = 0.0, b = 0.0;
    char op = 0;

    if (!(iss >> a))
        return ErrorCode::BadFormat;

    if (!(iss >> op)) {
        *out = a;
        return ErrorCode::OK;
    }

    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    if (op == '!') {
        std::string rest;
        if (iss >> rest)
            return ErrorCode::BadFormat;
        return operations['!'](a, 0, out);
    }

    if (!(iss >> b))
        return ErrorCode::BadFormat;

    // dodatkowe operatory po drugiej liczbie
    std::string rest;
    if (iss >> rest)
        return ErrorCode::BadFormat;

    return operations[op](a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
