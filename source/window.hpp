#ifndef _WINDOWS_
#include <Windows.h>
#endif

#define SDL_MAIN_HANDLED // why the FUCK did they think this was a good idea........ igfdhkjghfdkjg

#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif

#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>


// This is NOT the window struct for applications. This is the internal SDL window class.
struct InternalWindow {
    struct WinData{ // Data for the internal window + taskbar
        
    };
    
    int Runner();
    static void killExplorer() {
        // This is something that I should absolutely NEVER do, but it's the only way to kill explorer.exe taskbar & desktop that keeps it killed.
        const char* taskkill_ = "taskkill /F /IM explorer.exe"; // If this is changed, then you've got more to worry about than a system() call.... You need an antivirus!
        // I'll do a few checks anyways.
        system(taskkill_);
        return;
    }
    static int killProcName(const char* name) {
        PROCESSENTRY32 entry;
        entry.dwFlags = sizeof( PROCESSENTRY32 );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if ( Process32First( snapshot, &entry ) == TRUE ) {
            while ( Process32Next( snapshot, &entry ) == TRUE ) {
                if ( _stricmp( entry.szExeFile, "explorer.exe" ) == 0 ) {
                    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, entry.th32ProcessID );

                    TerminateProcess( hProcess, 0 );

                    CloseHandle( hProcess );
                    return 0;
                    break;
                }
            }

        }
        return -1;
    }
};