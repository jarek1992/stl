#include <iostream>
#include "is_palindrome.hpp"

int main() {

	std::string test1 = "Hello, world!";
	is_palindrome(test1);

	std::cout << std::boolalpha;
    std::cout << "\"" << test1 << "\" -> " << is_palindrome(test1) << "\n";

	return 0;
}