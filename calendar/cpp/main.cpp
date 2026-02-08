#include <iostream>
#include <array>
#include <string>
#include <format>

#include "zellers.h"
#include "calendar.h"

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

    int day = zellers_congruence(inputs);

    
    print_month(day, month, year);

    return 0;
}