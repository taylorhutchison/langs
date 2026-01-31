#pragma once

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #define PLATFORM_POSIX
#else
    #error "Unknown platform"
#endif

#include <iostream>

// Function declarations
void set_cursor_position(int x, int y);
void clear_console();