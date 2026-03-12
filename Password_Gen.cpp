#include <iostream>
#include <string>
#include <random>
#include <stdexcept>
#include <algorithm>

class PasswordGenerator {
private:
    const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    const std::string numbers = "0123456789";
    const std::string symbols = "!@#$%^&*()-_=+[]{}|;:,.<>?";
    const std::string all_chars = uppercase + lowercase + numbers + symbols;

public:
    std::string generate(int length) {
        if (length < 8) {
            throw std::invalid_argument("Minimum password length must be at least 8.");
        }

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

        pick_random(uppercase, required_each);
        pick_random(lowercase, required_each);
        pick_random(numbers, required_each);
        pick_random(symbols, required_each);

        int remaining_length = length - (required_each * 4);
        std::uniform_int_distribution<size_t> dist_all(0, all_chars.size() - 1);

        for (int i = 0; i < remaining_length; ++i) {
            password += all_chars[dist_all(generator)];
        }

        std::shuffle(password.begin(), password.end(), generator);

        return password;
    }
};

int main() {
    PasswordGenerator generator;
    int length;

    std::cout << "--- Advanced C++ Password Generator ---\n";
    std::cout << "Enter length (Slider minimum is 8): ";
    std::cin >> length;

    try {
        std::string securePassword = generator.generate(length);
        std::cout << "\nYour secure password is: " << securePassword << "\n";

        std::cout << "(Generated length: " << securePassword.length() << " characters)\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
