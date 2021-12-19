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
     * @brief Get the HWNDs From ProcessID
     * 
     * @param dwProcessID 
     * @param out : Returns vector of HWNDs associated with dwProcessID, or an empty vector if none is found.
     */
    static void GetWinFromPID(DWORD dwProcessID, std::vector<HWND>& out)
    {
        HWND hCurWnd = NULL;
        do
        {
            hCurWnd = FindWindowEx(NULL, hCurWnd, NULL, NULL);
            DWORD dwProcID = 0;
            GetWindowThreadProcessId(hCurWnd, &dwProcID);
            if (dwProcID == dwProcessID)
            {
                out.push_back(hCurWnd);
            }
        }
        while (hCurWnd != NULL);

        if (hCurWnd == NULL) {
            out.clear(); // If this is called, it means all windows were null, or none were found.
        }
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
                GetWinFromPID(GetProcessId(hProcess), temp);
                if (temp.size() == 0) { // If the size is 0, it means no HWNDs are associated with this hProcess
                    continue; // Break out of this part of the loop
                }
                InternalWindow::instance->procHWNDs.insert(InternalWindow::instance->procHWNDs.end(), temp.begin(), temp.end());

            }
        }
        return ( & InternalWindow::GetInstance()->procHWNDs );
    }

    /**
     * @brief Kills process by name, first instance
     * 
     * @param name : name of process (.exe name) 
     * @return int 
     */
    static int killProcName(const char* name) {
        PROCESSENTRY32 entry;
        entry.dwFlags = sizeof( PROCESSENTRY32 );

        HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

        if ( Process32First( snapshot, &entry ) == TRUE ) {
            while ( Process32Next( snapshot, &entry ) == TRUE ) {
                if ( _stricmp( entry.szExeFile, name ) == 0 ) {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, entry.th32ProcessID );

                    TerminateProcess( hProcess, 0 );

                    CloseHandle( hProcess );
                    return 0;
                    break;
                }
            }

        }
        return -1;
    }
    
    /**
     * @brief Forcibly kills process by handle
     * 
     * @param handle : handle to kill
     * @return int   : return value
     */
    static int killProcHandle(HANDLE handle) {
        TerminateProcess( handle, 1 );
        CloseHandle( handle );

        return -1;
    }

    
};