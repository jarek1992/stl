#include "arithmeticAverage.hpp"

#include <iostream>

int main() {

    std::vector<int> first{-5, 4, 2, -5, 2, 4, 6, 7}; 
    std::vector<int> second{3, 5, -2, 6, 7, -3, 6, 9};

    double avg = ArithmeticAverage(first, second);
    std::cout << "arithemitc average: " << avg << std::endl;
    
    double dist = Distance(first, second);
    std::cout << "distance between 2 vectors: " << dist << std::endl;

    return 0;
}