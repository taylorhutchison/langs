#include <iostream>
#include <array>
#include <string>

struct ZellersInputs {
    int day;
    int month;
    int year;
    int century;
};

constexpr int zellers_congruence(const ZellersInputs& inputs) {
    return (inputs.day +
            (13 * (inputs.month + 1) / 5) +
            inputs.year +
            (inputs.year / 4) +
            (inputs.century / 4) - (2 * inputs.century)) % 7;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <month> <year>" << std::endl;
        return 1;
    }

    int month = 0;
    int year = 0;
    try {
        month = std::stoi(argv[1]);
        year = std::stoi(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Invalid input: " << e.what() << std::endl;
        return 1;
    }

    if (month < 1 || month > 12) {
        std::cerr << "Month must be between 1 and 12." << std::endl;
        return 1;
    }

    ZellersInputs inputs {
        1,
        (month <= 2) ? (month + 12) : month,
        (month <= 2) ? (year - 1) % 100 : (year % 100),
        year / 100
    };

    int n = zellers_congruence(inputs);

    constexpr std::array<const char*, 7> days = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    std::string day_name = (n >= 0 && n <= 6) ? days[n] : "Invalid";

    std::cout << "Month " << month << " of year " << year << " starts on " << day_name << std::endl;
    return 0;
}
