#pragma once

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