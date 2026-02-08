#include <iostream>
#include <array>
#include <string>
#include <format>

#include "calendar.h"

void print_month(int day_of_week, int month, int year)
{
    std::string day_name = (day_of_week >= 0 && day_of_week <= 6) ? days[day_of_week] : "Invalid";
    std::string month_name = (month >= 1 && month <= 12) ? months[month - 1] : "Invalid";

    std::cout << std::format("{:>41}", month_name + " " + std::to_string(year)) << std::endl;
    
    auto shifted_day = (day_of_week + 5 ) % 7;

    std::cout << std::format("{:<5} {:<5} {:<5} {:<5} {:<5} {:<5} {:<5}\n",
                             "| Mon", "| Tue", "| Wed", "| Thu", "| Fri", "| Sat", "| Sun");


    auto is_leap_year = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    auto max_days = days_in_month(month, is_leap_year) + shifted_day;
    for (auto i = 1; i <= max_days; i++)
    {
        if(i > shifted_day) {
            std::cout << std::format("{:<6}", std::format("| {:02}", i - shifted_day));
        } else {
            std::cout << std::format("{:<6}", "| ");
        }
        if(i % 7 == 0) std::cout << std::endl;
    }
}