#include <windows.h>
#include "terminal_util.h"

void setWarna(int warna) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, warna);
}

void resetWarna() {
    setWarna(WARNA_DEFAULT);
}
