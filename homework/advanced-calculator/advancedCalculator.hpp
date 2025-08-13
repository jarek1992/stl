#include <map>
#include <functional>
#include <string>
#include <variant>

enum class ErrorCode {
	Ok,
	BadCharacter,
	BadFormat,
	DividedBy0,
    SqrtOfNegativeNumber,
    ModuleOfNonIntegerValue
};

std::string errorCodeToString(ErrorCode code);

class AdvancedCalculator {
public:
	using Result = std::variant<double, ErrorCode>;

	AdvancedCalculator();
    Result calculate(char op, double a, double b = 0) const;

private:
    std::map<char, std::function<Result(double, double)>> operations;

};