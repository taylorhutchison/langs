#include <iostream>
#include <cstdlib>

int zellers_congruence(int day_of_month, int month, int year_of_century, int century) {
    return (day_of_month + 
        (13 * (month + 1) / 5) + 
        year_of_century + 
        (year_of_century / 4) +
        (century / 4) - (2 * century)) % 7;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <month> <year>" << std::endl;
        return 1;
    }

    int month = std::atoi(argv[1]);
    int year = std::atoi(argv[2]);

    if (month <= 2) {
        month += 12;
        year -= 1;
    }

    int year_of_century = year % 100;
    int century = year / 100;

    int n = zellers_congruence(1, month, year_of_century, century);

    const char* days[] = { "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday" };
    std::string day_name = (n >= 0 && n <= 6) ? days[n] : "Invalid";

    std::cout << "Month: " << month << ", Year: " << year << "Starts on day: " << day_name << std::endl;

    return 0;
}

