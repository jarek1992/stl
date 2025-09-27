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
    enum class State { Start,
                       Number,
                       Operator };
    State state = State::Start;

    double a = 0, b = 0;
    char op = 0;

    std::istringstream iss(input);
    std::string token;

    // Wczytaj pierwszą liczbę (może mieć znak i kropkę)
    if (!(iss >> token))
        return ErrorCode::BadFormat;

    // Sprawdź format liczby
    bool dotUsed = false;
    for (size_t i = 0; i < token.size(); ++i) {
        char c = token[i];
        if (c == '.') {
            if (dotUsed)
                return ErrorCode::BadFormat;
            dotUsed = true;
        } else if (c == '+' || c == '-') {
            if (i != 0)
                return ErrorCode::BadFormat;  // znak tylko na początku
        } else if (!std::isdigit(c)) {
            return ErrorCode::BadCharacter;
        }
    }

    try {
        a = std::stod(token);
    } catch (...) {
        return ErrorCode::BadFormat;
    }

    // Wczytaj operator
    if (!(iss >> op))
        return ErrorCode::BadFormat;

    if (operations.find(op) == operations.end())
        return ErrorCode::BadCharacter;

    // Operator jednoargumentowy (!) nie wymaga drugiej liczby
    if (op != '!') {
        if (!(iss >> token))
            return ErrorCode::BadFormat;

        dotUsed = false;
        for (size_t i = 0; i < token.size(); ++i) {
            char c = token[i];
            if (c == '.') {
                if (dotUsed)
                    return ErrorCode::BadFormat;
                dotUsed = true;
            } else if (c == '+' || c == '-') {
                if (i != 0)
                    return ErrorCode::BadFormat;
            } else if (!std::isdigit(c)) {
                return ErrorCode::BadCharacter;
            }
        }

        try {
            b = std::stod(token);
        } catch (...) {
            return ErrorCode::BadFormat;
        }
    }

    // Sprawdź, czy reszta strumienia jest pusta lub same spacje
    std::string rest;
    if (std::getline(iss, rest)) {
        for (char c : rest) {
            if (!std::isspace(c))
                return ErrorCode::BadFormat;
        }
    }

    // Wywołanie funkcji operacji
    return operations.at(op)(a, b, out);
}

ErrorCode process(const std::string& input, double* out) {
    AdvancedCalculator calc;
    return calc.process(input, out);
}
