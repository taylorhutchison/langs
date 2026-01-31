#include <iostream>
#include <bitset>
#include <random>
#include <thread>
#include <chrono>

constexpr int num_columns = 40;
constexpr int num_rows = 20;
constexpr int grid_size = num_columns * num_rows;

std::bitset<grid_size> grid1;
std::bitset<grid_size> grid2;

void randomize_bitset(std::bitset<grid_size>& grid) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 1);

    for (size_t i = 0; i < grid_size; ++i) {
        grid[i] = dist(gen);
    }
}

int number_of_neighbors(const std::bitset<grid_size>& grid, int row, int column)
{
    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            int newRow = row + i;
            int newCol = column + j;
            if (i == 0 && j == 0) continue;
            if (newRow >= 0 && newRow < num_rows && newCol >= 0 && newCol < num_columns)
            {
                if (grid[newRow * num_columns + newCol]) count++;
            }
        }
    }
    return count;
}

void update_grid(const std::bitset<grid_size>& current, std::bitset<grid_size>& next)
{
    for (int row = 0; row < num_rows; row++)
    {
        for (int column = 0; column < num_columns; column++)
        {
            int count = number_of_neighbors(current, row, column);
            int idx = row * num_columns + column;
            if (current[idx])
            {
                next[idx] = count == 2 || count == 3;
            }
            else
            {
                next[idx] = count == 3;
            }
        }
    }
}

// Platform-specific cursor positioning
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void set_cursor_position(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
}
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#include <cstdio>
void set_cursor_position(int x, int y) {
    //ANSI escape code for cursor position (1-based)
    std::printf("\033[%d;%dH", y + 1, x + 1);
    std::fflush(stdout);
}
#endif

void clear_console() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void print_grid(const std::bitset<grid_size>& grid) {
    set_cursor_position(0,0);
    for(int row = 0; row < num_rows; row++) {
        for(int column = 0; column < num_columns; column++) {
            std::cout << (grid[row * num_columns + column] ? "X" : " ");
        }
        std::cout << std::endl;
    }
}



int main()
{
    clear_console();
    randomize_bitset(grid1);
    std::bitset<grid_size>* current = &grid1;
    std::bitset<grid_size>* next = &grid2;
    while(true) {
        print_grid(*current);
        update_grid(*current, *next);

        std::swap(current, next);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}