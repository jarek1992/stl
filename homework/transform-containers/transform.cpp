#include <iterator>
#include <vector>

#include "transform.hpp"

std::map<int, std::string> removeDuplicateAndTranformToMap(
	const std::list<std::string>& values, 
	const std::deque<int>& keys) {

	std::vector<std::pair<int, std::string>> temp;
    temp.reserve(std::min(keys.size(), values.size()));

	std::transform(keys.begin(), keys.end(), values.begin(), std::back_inserter(temp), [](int key, const std::string& val) {
        return std::make_pair(key, val);
    });

	std::map<int, std::string> result;
    for (const auto& [key, val] : temp) {
		result.try_emplace(key, val);
    }

	return result;
}