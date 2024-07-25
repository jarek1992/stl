#include "sort.hpp"
#include <algorithm>

std::deque<std::string> lengthSort(const std::forward_list<std::string>& words) {
    std::deque<std::string> sorted;
    for (const auto& word : words) {
        sorted.push_back(word);
    }

    std::sort(sorted.begin(), sorted.end(), [](const std::string& a, const std::string& b) {
        if (a.length() == b.length()) {
            return a < b;
        }
        return a.length() < b.length();
    });
    return sorted;
}