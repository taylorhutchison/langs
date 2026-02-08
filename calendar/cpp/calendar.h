#pragma once

#include <array>

constexpr std::array<const char *, 7> days = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
constexpr std::array<const char *, 12> months = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

constexpr int days_in_month(int month, bool is_leap_year) {
    return (month == 2) ? (is_leap_year ? 29 : 28)
         : (month == 4 || month == 6 || month == 9 || month == 11) ? 30
         : 31;
}

void print_month(int day_of_week, int month, int year);