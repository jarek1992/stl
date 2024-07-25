#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <cstdint>

constexpr size_t width = 32;
constexpr size_t height = 32;

using compressedGrayscaleImage = std::vector<std::pair<uint8_t, uint8_t>>;
using grayScaleImage = std::array<std::array<uint8_t, width>, height>;

compressedGrayscaleImage compressGrayscale(const grayScaleImage& bitmap);
grayScaleImage decompressGrayscale(const compressedGrayscaleImage& compressed);
void printMap(const grayScaleImage& bitmap);
