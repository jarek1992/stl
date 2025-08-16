#pragma once

#include <functional>
#include <map>
#include <string>
#include <variant>

enum class ErrorCode {
    OK,
    BadCharacter,
    BadFormat,
    DivideBy0,
    SqrtOfNegativeNumber,
    ModuleOfNonIntegerValue
};

std::string errorCodeToString(ErrorCode code);

class AdvancedCalculator {
public:
    AdvancedCalculator();
    ErrorCode process(const std::string& input, double* out);

private:
    std::map<char, std::function<ErrorCode(double, double, double*)>> operations;
};

ErrorCode process(const std::string& input, double* out) const;

