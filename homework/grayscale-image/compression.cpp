#include <algorithm>
#include <iterator>
#include <algorithm>

#include "compression.hpp"

std::vector<std::pair<uint8_t, uint8_t>>compressGrayscale(const std::array<std::array<uint8_t, width>, height>& bitmap) {
    std::vector<std::pair<uint8_t, uint8_t>> compressed;
    compressed.reserve(width * height);

    for(const auto& row : bitmap) {
        auto it = row.begin();

        while(it != row.end()) {
            uint8_t current_value = *it;
            auto run_end = std::find_if(it, row.end(),
                                        [current_value](uint8_t val) { 
                    return val != current_value; 
            });

            size_t count = std::distance(it, run_end);

            while (count > 0) {
                uint8_t chunk = static_cast<uint8_t>(std::min(count, static_cast<size_t>(255)));
                compressed.emplace_back(current_value, chunk);
                count -= chunk;
            }

            it = run_end;
        }
    }
    compressed.shrink_to_fit();
    return compressed;
}

std::array<std::array<uint8_t, width>, height> decompressGrayscale(const std::vector<std::pair<uint8_t, uint8_t>>& compressed) {
    std::array<std::array<uint8_t, width>, height> decompressed = {};
    size_t pos = 0;

    for (const auto& [value, count] : compressed) {
        uint8_t* start = &decompressed[0][0] + pos;

        std::fill_n(start, count, value);

        pos += count;
       
    }
    return decompressed;
}

void printMap(const std::array<std::array<uint8_t, width>, height>& bitmap) {
    for(const auto& row : bitmap) {
        std::string line;
        line.reserve(width);

        std::transform(row.begin(), row.end(), std::back_inserter(line), [](uint8_t el) {
            return (el < 32) ? ' ' : static_cast<char>(el);
        });

        std::cout << std::endl;
    }
}