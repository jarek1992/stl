#include <iostream>

#include "transform.hpp"

int main() {
	std::list<std::string> list{
        "Ala", "Kot", "Ma", "Rysia", "Ala",
        "Sierotka", "Kot", "Ma", "Ala"};
    std::deque<int> deque{
        1, 2, 3, 4, 5, 3, 1, 2, 3, 4,
        5, 2, 3, 1, 1, 2, 3, 2, 1, 4};

	  std::map<int, std::string> result = removeDuplicateAndTranformToMap(list, deque);

    for (const auto& [key, value] : result) {
          std::cout << key << ", " << value << '\n';
    }

	return 0;
}
