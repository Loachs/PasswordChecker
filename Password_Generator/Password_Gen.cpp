#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
#include <algorithm>

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
#endif

class PasswordGenerator {
private:
    std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    std::string numbers   = "0123456789";
    std::string symbols   = "!@#$%^&*()-_=+[]{}|;:,.<>?";

    const std::string ambiguous = "Il1O0"; 

    void removeAmbiguous(std::string& pool) {
        pool.erase(std::remove_if(pool.begin(), pool.end(),
            [&](char c) { return ambiguous.find(c) != std::string::npos; }), 
            pool.end());
    }

public:
    std::string generate(int length, bool excludeAmbiguous) {
        if (length < 8) {
            throw std::invalid_argument("Minimum password length must be at least 8.");
        }

        std::string active_upper = uppercase;
        std::string active_lower = lowercase;
        std::string active_numbers = numbers;
        std::string active_symbols = symbols;

        if (excludeAmbiguous) {
            removeAmbiguous(active_upper);
            removeAmbiguous(active_lower);
            removeAmbiguous(active_numbers);
            removeAmbiguous(active_symbols);
        }

        std::string all_chars = active_upper + active_lower + active_numbers + active_symbols;
        int required_each = (length >= 12) ? 2 : 1;

        std::random_device rd; 
        std::mt19937_64 generator(rd()); 
        std::string password;
        password.reserve(length);

        auto pick_random = [&](const std::string& pool, int count) {
            std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
            for (int i = 0; i < count; ++i) {
                password += pool[dist(generator)];
            }
        };

        pick_random(active_upper, required_each);
        pick_random(active_lower, required_each);
        pick_random(active_numbers, required_each);
        pick_random(active_symbols, required_each);

        int remaining_length = length - (required_each * 4);
        std::uniform_int_distribution<size_t> dist_all(0, all_chars.size() - 1);
        
        for (int i = 0; i < remaining_length; ++i) {
            password += all_chars[dist_all(generator)];
        }

        std::shuffle(password.begin(), password.end(), generator);

        return password;
    }
};

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(password_module) {
    emscripten::class_<PasswordGenerator>("PasswordGenerator")
        .constructor<>()
        .function("generate", &PasswordGenerator::generate);
}
#else
int main() {
    PasswordGenerator generator;
    int length = 14;

    std::cout << "Generated: " << generator.generate(length, true) << "\n";
    return 0;
}
#endif
