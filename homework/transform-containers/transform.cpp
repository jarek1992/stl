#include <iterator>
#include <vector>
#include<set>

#include "transform.hpp"

std::map<int, std::string> removeDuplicateAndTranformToMap(
	std::list<std::string>& values, 
	std::deque<int>& keys) {

	//remove duplicates
    std::set<std::string> uniqueStrings(values.begin(), values.end());
    std::set<int> uniqueInts(keys.begin(), keys.end());

	//rewrite to the additional containers
    std::vector<std::string> uniqueStrVec(uniqueStrings.begin(), uniqueStrings.end());
    std::vector<int> uniqueIntVec(uniqueInts.begin(), uniqueInts.end());

	//adjust length
	size_t count = std::min(uniqueStrVec.size(), uniqueIntVec.size());
    uniqueStrVec.resize(count);
    uniqueIntVec.resize(count);

	//create a map with std::transform
    std::map<int, std::string> result;
    std::transform(
        uniqueIntVec.begin(), uniqueIntVec.end(), 
		uniqueStrVec.begin(),
        std::inserter(result, result.begin()),
		[](int key, const std::string& val) {
			return std::make_pair(key, val);
		});

	return result;
}