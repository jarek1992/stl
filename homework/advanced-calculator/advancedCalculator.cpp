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

// ErrorCode AdvancedCalculator::process(const std::string& input, double* out) {
//     if (input.empty())
//         return ErrorCode::BadFormat;
//
//     // sprawdzenie niedozwolonych znaków
//     for (char c : input) {
//         if (!(std::isdigit(c) || std::isspace(c) || c == '.' || operations.count(c))) {
//             return ErrorCode::BadCharacter;
//         }
//     }
//
//     std::istringstream iss(input);
//     double a = 0.0, b = 0.0;
//     char op = 0;
//
//     // pierwsza liczba
//     if (!(iss >> a))
//         return ErrorCode::BadFormat;
//
//     // operator
//     if (!(iss >> op)) {
//         *out = a;  // tylko liczba
//         return ErrorCode::OK;
//     }
//
//     // nieznany operator
//     if (operations.find(op) == operations.end())
//         return ErrorCode::BadCharacter;
//
//     // operator unarny
//     if (op == '!') {
//         std::string extra;
//         if (iss >> extra)
//             return ErrorCode::BadFormat;  // np. "5! 2"
//         return operations['!'](a, 0, out);
//     }
//
//     // druga liczba
//     if (!(iss >> b))
//         return ErrorCode::BadFormat;
//
//     // sprawdź, czy po drugiej liczbie coś jeszcze jest
//     std::string rest;
//     if (iss >> rest)
//         return ErrorCode::BadFormat;
//
//     // oblicz wynik
//     return operations[op](a, b, out);
// }

ErrorCode AdvancedCalculator::process(const std::string& input, double* out) {
    if (input.empty())
        return ErrorCode::BadFormat;

    // sprawdzenie niedozwolonych znaków i złych formatów liczbowych
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];

        if (c == ',') {
            // przecinek między cyframi -> zły format
            bool beforeDigit = (i > 0 && std::isdigit(input[i - 1]));
            bool afterDigit = (i + 1 < input.size() && std::isdigit(input[i + 1]));
            if (beforeDigit && afterDigit)
                return ErrorCode::BadFormat;
            else
                return ErrorCode::BadCharacter;
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

    // przypadek: liczba zawiera więcej niż jedną kropkę (np. "12.4.3")
    std::string check = std::to_string(a);
    size_t dotCount = std::count(input.begin(), input.end(), '.');
    if (dotCount > 1 && input.find_first_of("+-*/%^$!") != std::string::npos)
        return ErrorCode::BadFormat;

    if (!(iss >> op)) {
        *out = a;
        return ErrorCode::OK;
    }

    // nieznany operator
    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    // operator unarny (!)
    if (op == '!') {
        std::string extra;
        if (iss >> extra)
            return ErrorCode::BadFormat;  // np. "123.4 ! 345"
        return operations['!'](a, 0, out);
    }

    // dwuznakowe operatory, np. ++ lub ^%
    char nextChar = 0;
    if (iss >> std::ws && iss.peek() != EOF) {
        nextChar = iss.peek();
        if (operations.count(nextChar) && nextChar != '-')  // drugi operator po operatorze
            return ErrorCode::BadFormat;
    }

    if (!(iss >> b))
        return ErrorCode::BadFormat;

    // coś jeszcze po drugiej liczbie
    std::string rest;
    if (iss >> rest)
        return ErrorCode::BadFormat;

    return operations[op](a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
