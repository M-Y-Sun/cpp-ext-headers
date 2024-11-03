#include <cstdio>

#include "../../include/misc.hh"

namespace ext
{

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <Windows.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/ioctl.h>
#endif

void
get_winsz (int *rows, int *cols)
{
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
    *rows = (int)(csbi.srWindow.Right - csbi.srWindow.Left + 1);
    *cols = (int)(csbi.srWindow.Bottom - csbi.srWindow.Top + 1);
#elif defined(__linux__) || defined(__APPLE__)
    struct winsize w;
    ioctl (fileno (stdout), TIOCGWINSZ, &w);
    *rows = (int)(w.ws_row);
    *cols = (int)(w.ws_col);
#endif
}

} // namespace ext
