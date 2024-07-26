#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <cstdint>

constexpr size_t width = 32;
constexpr size_t height = 32;

std::vector<std::pair<uint8_t, uint8_t>>compressGrayscale(const std::array<std::array<uint8_t, width>, height>& bitmap);
std::array<std::array<uint8_t, width>, height> decompressGrayscale(const std::vector<std::pair<uint8_t, uint8_t>>& compressed);
void printMap(const std::array<std::array<uint8_t, width>, height>& bitmap);
