#include <algorithm>
#include <string>

void removeVowels(std::vector<std::string>& words) {
    auto isVowel = [](char c) {
        static const std::string vowels = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
        return vowels.find(c) != std::string::npos;
    };
    for (auto& word : words) {
        word.erase(std::remove_if(word.begin(), word.end(), isVowel), word.end());
    }
}
