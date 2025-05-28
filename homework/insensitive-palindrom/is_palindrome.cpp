#include <iostream>
#include <algorithm>

#include "is_palindrome.hpp"

bool is_palindrome(const std::string& input) {
    std::string merged;
    merged.reserve(input.size());

    for (char c : input) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            merged.push_back(std::tolower(static_cast<unsigned char>(c)));
        }   
    }
    return std::equal(merged.begin(), merged.begin() + merged.size() / 2, merged.rbegin());
}