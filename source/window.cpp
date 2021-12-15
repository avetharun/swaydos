#include "window.hpp"
#define ALIB_BINARY    // Binary manipulation and utilities
#include "ave_libs.hpp"

int InternalWindow::Runner() {
    SDL_Init( SDL_INIT_VIDEO );
    // See bottommost comment of this file.
    unsigned char runnerData = Binary16(00000000,00000000);



    while (true) {
        /** \todo */

    }
    return 0; // If this gets returned, it's safe to assume the background process was exited normally.. but that also means all SDL windows will be closed
}
























/**
 
(lower byte)
0       0       0       0       0       0       0       0
^ Ready to taskkill explorer.exe?
        ^ Ready to show desktop?
                ^ Ready to show panels?
                        ^ Is input grabbed? If so read the following bit
                                ^ ________Input values_________________
                                    @b 0: Cursor focus is on panels   |
                                    @b 1: Cursor focus is on desktop  |
                                        ^ not used
                                                ^ not used
                                                        ^ not used

═══════════════════════════════════════════════════════════════════════
(upper byte)
0       0       0       0       0       0       0       0
^ not used
        ^ not used
                ^ not used
                        ^ not used
                                ^ not used
                                        ^ not usued
                                                ^ not used
                                                        ^ not used

*   NOTE: All unused bits are padding and are used for future booleans, or in other functions.

*/