#include <iostream>
#include <chrono>
#include <iostream>

#include "ave_libs.hpp"

#include "window.hpp"
int main(int argc, const char *argv[])
{
    ProgramArguments pgv = ProgramArguments(argc, argv);
    
    InternalWindow win;
    return win.Runner();
}


