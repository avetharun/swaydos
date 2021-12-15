#include <iostream>
#include <chrono>
#include <iostream>
#define ALIB_BINARY    // Binary manipulation and utilities

#include "ave_libs.hpp"

#include "window.hpp"

int main() {
    
    
    // InternalWindow::killProcName("explorer.exe"); // If explorer.exe is alive, kill it. 
    // It will kill any file viewer explorer.exe's but that'll have to happen I guess..
    InternalWindow win;
    return win.Runner();
}


