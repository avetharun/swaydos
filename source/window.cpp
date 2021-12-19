#include "window.hpp"
#include "ave_libs.hpp"
#include <thread>
#include <functional>
#include <iostream>

#include "swayconfig.hpp"

std::function<void ()> PanelRunner;
int InternalWindow::Runner() {
    SDL_Init( SDL_INIT_VIDEO );
    // See bottommost comment of this file.
    unsigned short runnerData = Binary16(00000000,00000000);
    this->InternalPanelWindow =   SDL_CreateWindow("SDOS_PANEL",  0,0,  100,100,  SDL_WINDOW_BORDERLESS | -SDL_WINDOW_RESIZABLE);
    this->InternalDesktopWindow = SDL_CreateWindow("SDOS_DESK",   0,0,  100,100,  SDL_WINDOW_BORDERLESS | -SDL_WINDOW_RESIZABLE);

    InternalWindow::GetInstance()->InitThreadProc(); // Initialize threads and functions associated

    // Create thread for panel management
    InternalWindow::GetInstance()->panelThread = new std::thread(PanelRunner);
    InternalWindow::GetInstance()->panelThread->detach();
    
    while (true) {}

    
    //std::thread desktopThread;  Will work on this later.
    return 0; // If this gets returned, it's safe to assume the background process was exited normally.. but that also means all SDL windows will be closed
}

void InternalWindow::OnExit() {
    InternalWindow::GetInstance()->panelThread->join();

}

void InternalWindow::InitThreadProc() {

    PanelRunner = std::function([&] () {
        std::function<void()> RenderProcS = {
            
        };
        while (true) {
            
            /** \todo */
        }
    });
}




// Init externals
InternalWindow * InternalWindow::instance = nullptr;
std::thread * InternalWindow::bgThread = nullptr;
std::thread * InternalWindow::panelThread = nullptr;















/**
 
(lower byte)
0   0   0   0   0   0   0   0
^ Ready to taskkill explorer.exe?
    ^ Ready to show desktop?
        ^ Ready to show panels?
            ^ Is input grabbed? If so read the following bit
                ^ ______Input values_________________
                  @b 0: Cursor focus is on panels   |
                  @b 1: Cursor focus is on desktop  |
                    ^ not used
                        ^ not used
                            ^ not used

═══════════════════════════════════════════════════════════════════════
(upper byte)
0   0   0   0   0   0   0   0
^ not used
    ^ not used
        ^ not used
            ^ not used
                ^ not used
                    ^ not usued
                        ^ not used
                            ^ Process Exiting

*   NOTE: All unused bits are padding and are used for future booleans, or in other functions.

*/