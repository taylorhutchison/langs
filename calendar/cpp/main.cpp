#include <iostream>
#include <cstdlib>

struct ZellersInputs
{
    int day;
    int month;
    int year;
    int century;
};

int zellers_congruence(ZellersInputs inputs)
{
    return (inputs.day +
            (13 * (inputs.month + 1) / 5) +
            inputs.year +
            (inputs.year / 4) +
            (inputs.century / 4) - (2 * inputs.century)) % 7;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <month> <year>" << std::endl;
        return 1;
    }

    int month = std::atoi(argv[1]);
    int year = std::atoi(argv[2]);

    ZellersInputs inputs;

    inputs.day = 1;
    inputs.month = (month <= 2) ? (month + 12) : month;
    inputs.year = (month <= 2) ? (year - 1) % 100 : (year % 100);
    inputs.century = year / 100;

    int n = zellers_congruence(inputs);

    const char *days[] = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    std::string day_name = (n >= 0 && n <= 6) ? days[n] : "Invalid";

    std::cout << "Month " << month << " of year " << year << " starts on " << day_name << std::endl;

    return 0;
}
