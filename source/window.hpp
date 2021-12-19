#ifndef swaydos_windowmgr_mainw_hpp
#define swaydos_windowmgr_mainw_hpp
#ifndef _WINDOWS_
#include <Windows.h>
#endif

#define SDL_MAIN_HANDLED // why the FUCK did they think this was a good idea........ igfdhkjghfdkjg

#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif
#include <functional>
#include <vector>
#include <thread>


#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>

#include "ave_libs.hpp"

/**
 * @brief A panel that can either be floating, as a window of sorts, or on the Taskbar.
 * 
 */
struct Panel{
    unsigned short db = 0; // 16-bit data buffer to enable internalPassTransient to quickly see if anything has changed.
    int x;
    int y;
    int w;
    int h;
    char* name;
    std::function<void(unsigned long)> onClick;
    std::function<void(unsigned long)> onKeyPress;
    void internalPassTransient(); // Check buffers to see if anything has changed since the last check, should run every ~25ms
};

// This is NOT the window struct for applications. This is the internal SDL window class.
struct InternalWindow {
    bool exiting = false;
    static InternalWindow* instance;
    static InternalWindow* GetInstance() {  return (instance == nullptr) ? new InternalWindow() : instance;     }
    
    static std::vector<HANDLE> procHandles;
    static std::vector<HWND> procHWNDs;

    static std::thread * panelThread; // Panel thread
    static std::thread * bgThread;    // Background task thread

    SDL_Window* InternalPanelWindow;
    SDL_Window* InternalDesktopWindow;
    
    int Runner();
    void OnExit();
    void OnKill();
    void InitThreadProc();

    static void killExplorer() {
        // This is something that I should absolutely NEVER do, but it's the only way to kill explorer.exe taskbar & desktop that keeps it killed.
        const char* taskkill_ = "taskkill /f /fi \"SESSION eq 1\" /im explorer.exe"; // If this is compsomised, then you've got more to worry about than a system() call.... 
        
        // I'll do a few checks anyways, eventually.
        system(taskkill_);
        return;
    }
    /**
     * @brief Refreshes & returns all process handles
     * @return std::vector<HANDLE> *
     */
    static std::vector<HANDLE> * refreshProcHandles() {        
        PROCESSENTRY32 entry;
        entry.dwFlags = sizeof( PROCESSENTRY32 );
        InternalWindow::GetInstance()->procHandles.clear();
        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
        
        if ( Process32First( snapshot, &entry ) == TRUE ) {
            while ( Process32Next( snapshot, &entry ) == TRUE ) {
                HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, entry.th32ProcessID );
                InternalWindow::GetInstance()->procHandles.push_back(hProcess);   
            }
        }
        return ( & InternalWindow::GetInstance()->procHandles );
    }
    /**
     * @brief Refreshes & returns all process HWNDs
     * 
     * @return std::vector<HWND> * 
     */
    static std::vector<HWND> * refreshProcWindows() {
        PROCESSENTRY32 entry;
        entry.dwFlags = sizeof( PROCESSENTRY32 );
        InternalWindow::GetInstance()->procHandles.clear();
        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
        // I hate using FALSE/TRUE/whatever because I prefer to use true/false in LOWERCASE fkhdsjhfs
        if ( Process32First( snapshot, &entry ) == TRUE ) {
            while ( Process32Next( snapshot, &entry ) == TRUE ) {
                // Request as many permissions as possible, to get as many HWNDs as possible.
                HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID );
                std::vector<HWND> temp;
                getWinFromPID(GetProcessId(hProcess), temp);
                if (temp.size() == 0) { // If the size is 0, it means no HWNDs are associated with this hProcess
                    continue; // Break out of this part of the loop
                }
                InternalWindow::instance->procHWNDs.insert(InternalWindow::instance->procHWNDs.end(), temp.begin(), temp.end());

            }
        }
        return ( & InternalWindow::GetInstance()->procHWNDs );
    }

    
};
#endif