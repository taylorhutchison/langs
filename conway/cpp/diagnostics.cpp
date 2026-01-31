#include "diagnostics.h"
#include "console.h"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <psapi.h>
#elif defined(PLATFORM_POSIX)
    #include <unistd.h>
    #include <fstream>
#endif

size_t get_working_set_size() {
#ifdef PLATFORM_WINDOWS
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    return 0;
#elif defined(PLATFORM_POSIX)
    long rss = 0L;
    std::ifstream statm("/proc/self/statm");
    if (statm) {
        long pages = 0;
        statm >> pages >> rss;
        statm.close();
        return rss * sysconf(_SC_PAGESIZE);
    }
    return 0;
#endif
}