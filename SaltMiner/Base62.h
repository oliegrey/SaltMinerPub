#pragma once
#include <unordered_map>
#include <string>


class Base62 {
private:
    inline static const std::string characters = "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

public:
    static bool isValidInput(std::string_view input) {
        if (input.size() != 4) { return false; }
        for (char c : input) {
            if (characters.find(c) == std::string::npos) { return false; }
        }
        return true;
    }

    static std::string encode(uint32_t number) {
        std::string result;

        while (number > 0) {
            result = characters[number % 62] + result;
            number /= 62;
        }

        return result.empty() ? "0" : result;
    }

    static uint32_t decode(std::string_view str) {
        std::unordered_map<char, int> charToValue;

        for (int i = 0; i < 62; ++i) { charToValue[characters[i]] = i; }

        uint32_t result = 0;
        for (char c : str) { result = result * 62 + charToValue[c]; }

        return result;
    }
};