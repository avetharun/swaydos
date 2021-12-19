#include "window.hpp"
#include "ave_libs.hpp"
#include <thread>
#include <functional>
#include <iostream>

#include "swayconfig.hpp"

std::function<void ()> swayMsgHandler;
int InternalWindow::Runner() {
    SDL_Init( SDL_INIT_VIDEO );
    // See bottommost comment of this file.
    unsigned short runnerData = Binary16(00000000,00000000);
    this->InternalPanelWindow =   SDL_CreateWindow("SWAYDOS_PANEL",  0,0,  100,100,  SDL_WINDOW_BORDERLESS | -SDL_WINDOW_RESIZABLE);
    this->InternalDesktopWindow = SDL_CreateWindow("SWAYDOS_DESK",   0,0,  100,100,  SDL_WINDOW_BORDERLESS | -SDL_WINDOW_RESIZABLE);
    InternalWindow::GetInstance()->InitThreadProc(); // Initialize threads and functions associated with them

    std::thread swayMessageHandler(swayMsgHandler);
    
    while (true) {}

    
    //std::thread desktopThread;  Will work on this later.
    return 0; // If this gets returned, it's safe to assume the panels have started normally.
}

void InternalWindow::OnExit() {
    InternalWindow::exiting = true;
    InternalWindow::GetInstance()->panelThread->join();
    //InternalWindow::GetInstance()->desktopThread->join();

}

void InternalWindow::InitThreadProc() {

    InternalWindow::GetInstance()->panelThread = new std::thread(
        std::function([&] () {

            // i've no idea what im doing... help me

            std::function<void()> RenderProcS = {
                
            };
            while (true) {
                /** \todo */
            }
        })
    );
}




// Init externals, because compiler complains.
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