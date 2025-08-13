#include <map>
#include <functional>
#include <string>
#include <variant>

enum class ErrorCode {
	Ok,
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
    ErrorCode process(const std::string& input, double* out) const;

private:
    std::map<char, std::function<ErrorCode(double, double, double*)>> operations;

};