#include "arithmeticAverage.hpp"

#include <numeric>
#include <cmath>

double ArithmeticAverage(const std::vector<int>& first, const std::vector<int>& second) {
    if (first.empty() && second.empty()) {
        std::cout << "empty vectors" << std::endl;
   }

    long long sum = std::accumulate(first.begin(), first.end(), 0) +
                   std::accumulate(second.begin(), second.end(), 0);

    size_t count = first.size() + second.size();

    return static_cast<double>(sum) / count;
}

double Distance(const std::vector<int>& first, const std::vector<int>& second) {
    if (first.size() != second.size()) {
        std::cout << "Vectors have to be the same length" << std::endl;
    }

    long double sumSquares = 0.0;
    for (size_t i = 0; i < first.size(); ++i) {
        long double diff = static_cast<long double>(first[i]) - static_cast<long double>(second[i]);
        sumSquares += diff * diff;
    }
    return std::sqrt(sumSquares);
}