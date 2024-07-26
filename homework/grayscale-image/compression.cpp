#include <algorithm>
#include <iterator>

#include "compression.hpp"

std::vector<std::pair<uint8_t, uint8_t>>compressGrayscale(const std::array<std::array<uint8_t, width>, height>& bitmap) {
    std::vector<std::pair<uint8_t, uint8_t>> compressed;
    compressed.reserve(width * height);
    for(const auto& row : bitmap) {
        auto it = row.begin();
        while(it != row.end()) {
            uint8_t current_value = *it;
            size_t count = 0;
            while (it != row.end() && *it == current_value) {
                ++count;
                ++it;
            }
            compressed.emplace_back(current_value, static_cast<uint8_t>(count));
        }
    }
    compressed.shrink_to_fit();
    return compressed;
}

std::array<std::array<uint8_t, width>, height> decompressGrayscale(const std::vector<std::pair<uint8_t, uint8_t>>& compressed) {
    std::array<std::array<uint8_t, width>, height> decompressed = {};
    size_t i = 0, j = 0;
    for(const auto& pair : compressed) {
        uint8_t value = pair.first;
        uint8_t count = pair.second;

        for(size_t k = 0; k < count; ++k) {
            decompressed[i][j] = value;
            if(++j == width) {
                j = 0;
                ++i;
            }
        }
    }
    return decompressed;
}

void printMap(const std::array<std::array<uint8_t, width>, height>& bitmap) {
    for(const auto& row : bitmap) {
        for(const auto& el : row) {
            if(el < 32) {
                std::cout << ' ';
            } else {
                std::cout << static_cast<char>(el);
            }
        }
        std::cout << std::endl;
    }
}