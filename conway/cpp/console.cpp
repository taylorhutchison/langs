#include "console.h"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
#elif defined(PLATFORM_POSIX)
    #include <cstdio>
#endif

void set_cursor_position(int x, int y) {
#ifdef PLATFORM_WINDOWS
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(hConsole, pos);
#elif defined(PLATFORM_POSIX)
    std::printf("\033[%d;%dH", y + 1, x + 1);
    std::fflush(stdout);
#endif
}

void clear_console() {
#ifdef PLATFORM_WINDOWS
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}