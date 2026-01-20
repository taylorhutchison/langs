#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
#include "bpm.h"

int main()
{
    std::ifstream file("image.bmp");

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    int width = *reinterpret_cast<int*>(&header[18]);
    int height = *reinterpret_cast<int*>(&header[22]);
    int bitsPerPixel = *reinterpret_cast<short*>(&header[28]);

    std::cout << width << " x " << height << "\n";

    std::cout << "bits per pixel " << bitsPerPixel << "\n";

    int dataOffset = *reinterpret_cast<int*>(&header[10]);
    file.seekg(dataOffset, std::ios::beg);

    int row_padded = (width * 3 + 3) & (~3);

    std::map<std::tuple<unsigned char, unsigned char, unsigned char>, int> color_counts;
    std::vector<unsigned char> row(row_padded);

    for (int y = 0; y < height; y++) {
        file.read(reinterpret_cast<char*>(row.data()), row_padded);
        for (int x = 0; x < width; ++x) {
            unsigned char blue = row[x * 3];
            unsigned char green = row[x * 3 + 1];
            unsigned char red = row[x * 3 + 2];
            color_counts[{red, green, blue}]++;
        }
    }

    std::vector<std::pair<std::tuple<unsigned char, unsigned char, unsigned char>, int>> sorted_colors(
        color_counts.begin(), color_counts.end());

    std::sort(sorted_colors.begin(), sorted_colors.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second; // Sort descending by count
        });

    std::cout << "Top 10 most used colors:\n";
    for (size_t i = 0; i < std::min<size_t>(10, sorted_colors.size()); ++i) {
        auto [r, g, b] = sorted_colors[i].first;
        int count = sorted_colors[i].second;
        std::cout << "R: " << (int)r << " G: " << (int)g << " B: " << (int)b
                  << " - Count: " << count << std::endl;
    }
    


    file.close();

    return 0;
}