#ifndef __lib_aveth_utils_hpp
#define __lib_aveth_utils_hpp

/**  //
//ㅤ//
/////
 @copyright Avetharun 2021 - GNU-GPL3-a
 Let's keep this simple. It follows the GNU GPL3 license, with very few modifications and exceptions.


 @b Youㅤcan:
    - Use this commercially
    - Distribute modified versions
    - Modify this file and/or projects
    - Patent projects that use this file
    - Use this file and/or project privately, ie. for use in an internal server (See limitations & Exceptions)
    - Use freely under the following conditions

 @b Conditions:
    - You must disclose the source file and/or author when distributing this file
    - A copy of this license must be attached to the file, in verbatim
    - The same (or similar) license must be used if you modify and license your version
    - Changes must be listed. ie. modifying source code must have your name and change in the file.
        - To follow this it must either be 
        A: Beside the change (on top or on bottom, in a comment)
        B: In the AUTHORs section of the file/project
        C: in any changelog that references the file's changes


 @b Limitationsㅤ/ㅤWhatㅤyouㅤcannotㅤdo
    - The user (you) are not liable for the harmful actions this program or file may cause, if any.
    - Keep code closed source (*See exceptions)


 @b Exceptions
    - If code is closed source, it must be in a private setting. Examples are as follows:
    EXA: A server used to host and/or distribute files
    EXB: Used as a base for porting and/or in microcontrollers
 */





#ifndef ALIB_NO_BINARY
#ifndef ALIB_BINARY_IMPL_ONCE
#define ALIB_BINARY_IMPL_ONCE
// 
//      Binary & bit manipulation utilities
//    - Avetharun
// 


/* Helper macros. Unless you know what you're doing, don't run these. */

// Binary decoding to unsigned long v
#define HEX__(n) 0x##n##LU
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)
// is this nonsense to anyone else? haha... hah.. heh....... help.
// mf c++ magic heh

// Binary encoding into hex (internal+user-runnable)
#define B8(d) ((unsigned char)B8__(HEX__(d)))
#define B16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
#define B32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
/** \todo B64(v,v,v,v,v,v,v,v)
 * but that'll be a mess...
 */

// Internal bit manipulation
#define imp_bitenb(val,n)   ( ( val ) |=  (1<<(n))) // Sets bit n to 1
#define imp_bitdis(val,n)   ( ( val ) &= ~(1<<(n))) // Sets bit n to 0
#define imp_flipbit(val,n)  ( ( val ) ^=  (1<<(n))) // Inverts bit n
#define imp_getbit(val,n)   ( ( val ) &   (1<<(n))) // Gets bit n from data val





/*---------------------------------------*/
/* User macros */

// Binary number creation to unsigned long


// Bit manipulation 
//
// Sets bit nbit to value bv in variable var
#define bitset(var,nbit,bv) (val > 0) ? imp_bitenb(var,nbit) : imp_bitdis(var,nbit)
// Sets bit nbit to value bv in variable var
#define setbit(var,nbit,bv)             bitset(var,nbit,bv) // alias of bitset(...) | See bitset(...) macro
// Sets bit nbit to TRUE/1
#define bitenable(var,nbit)             imp_bitenb(var,nbit)
// Sets bit nbit to FALSE/0
#define bitdisable(var,nbit)            imp_bitdis(var,nbit)
// Get bit nbit from value var
#define getbit(var,nbit)                imp_getbit(val,nbit)

// Bit creation
// 

/** 
 * Re-implementation of line @e 66 because compiler complains if I do the following:
 * #define Binary8(b8v) B8(b8v)
 * > Too many/little values in constant
*/
// Convert 8 bits into an unsigned char
#define Binary8(d) ((unsigned char)B8__(HEX__(d))) 
// Convert 16 bits (Each byte is seperated!) into an unsigned short
#define Binary16(dmsb,dlsb) (((unsigned short)B8(dmsb)<<8) \
+ B8(dlsb))
// Convert 32 bits (Each byte is seperated!) into an unsigned long
#define Binary32(dmsb,db2,db3,dlsb) (((unsigned long)B8(dmsb)<<24) \
+ ((unsigned long)B8(db2)<<16) \
+ ((unsigned long)B8(db3)<<8) \
+ B8(dlsb))
// Todo: Binary64 to T<long long>

/** 
 * @todo @b BinaryㅤReading. Effectively reversing the above functions.
 * Planned implementation: Eventually.
 */



#endif // NDEF(ALIB_BINARY_IMPL_ONCE)
#endif // NDEF(ALIB_NO_BINARY)
#ifndef ALIB_NO_WIN32_UTILS
#ifndef ALIB_WIN32_UTILS_IMPL_ONCE
#define ALIB_WIN32_UTILS_IMPL_ONCE
#include <Windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <vector>



/**
 * @brief Kills process by name, optionally by first instance
 * 
 * @param name  : name of process (.exe name) <const char*>
 * @param first : Should this kill only the first instance it finds? (true) 
 *                or should it kill all processes with the same name(false) (instances, etc.) <bool>
 * @return int  : status
 */
int killProcName(const char* name, bool first) {
    PROCESSENTRY32 entry;
    entry.dwFlags = sizeof( PROCESSENTRY32 );

    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );

    if ( Process32First( snapshot, &entry ) == TRUE ) {
        while ( Process32Next( snapshot, &entry ) == TRUE ) {
            if ( _stricmp( entry.szExeFile, name ) == 0 ) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, entry.th32ProcessID );

                TerminateProcess( hProcess, 0 );

                CloseHandle( hProcess );
                if (first) {
                    return 0;
                    break;
                }
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
int killProcHandle(HANDLE handle) {
    TerminateProcess( handle, 1 );
    CloseHandle( handle );

    return -1;
}

HANDLE getProcHandleName(const char* name) {
    PROCESSENTRY32 entry;
    entry.dwFlags = sizeof( PROCESSENTRY32 );
    HANDLE snapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
    // I hate using FALSE/TRUE/whatever because I prefer to use true/false in LOWERCASE fkhdsjhfs
    if ( Process32First( snapshot, &entry ) == TRUE ) {
        while ( Process32Next( snapshot, &entry ) == TRUE ) {
            if ( _stricmp( entry.szExeFile, name ) == 0 ) {
                return OpenProcess(PROCESS_ALL_ACCESS, false, entry.th32ProcessID );
            }
        }
    }
    return NULL;
}

/**
 * @brief Get the HWNDs From ProcessID
 * 
 * @param dwProcessID  Process ID to find windows for
 * @param out& : Returns vector of HWNDs associated with dwProcessID, or an empty vector if none is found.
 * @param ㅤ
 * @param Example:
 * @param 1 ㅤ std::vector<HWND> out;
 * @param 2 ㅤ GetWinFromPID(0, out);
 */
void getWinFromPID(DWORD dwProcessID, std::vector<HWND>& out)
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


#endif // NDEF(ALIB_WIN32_UTILS_IMPL_ONCE)
#endif // NDEF(ALIB_NO_WINFIND)
#ifndef ALIB_NO_FS_UTILS
#ifndef ALIB_FS_UTILS_IMPL_ONCE
#define ALIB_FS_UTILS_IMPL_ONCE
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
// If the OS supports POSIX functions, use this version of the file_exists function
#include <sys/stat.h>
// Fastest, but *NIX only.
inline bool file_exists (const char* name) {
    struct stat buffer;   
    return (stat (name, &buffer) == 0); 
}
#else
// Fallback to windows/crossplatform version
#include <string>
#include <fstream>
// Almost slowest, but hey, it's cross platform.
inline bool file_exists (const char* name) {
    FILE *file;
    if (fopen_s(&file, name, "r") != 0) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

#endif // UNIX + WIN32 checks
#endif // NDEF(ALIB_FS_UTILS_IMPL_ONCE)

#endif // NDEF(ALIB_NO_FS_UTILS)
#ifndef ALIB_NO_ARGPARSE
#ifndef ALIB_ARGPARSE_IMPL_ONCE
#define ALIB_ARGPARSE_IMPL_ONCE
#include <vector>
#include <map>
#include <string>

struct ProgramArguments{
    private:
    // splits a std::string into vector<string> at a delimiter
    std::vector<std::string> ALIB_PROG_ARG_split_string(std::string x, char delim = ' ')
    {
        x += delim; //includes a delimiter at the end so last word is also read
        std::vector<std::string> splitted;
        std::string temp = "";
        for (int i = 0; i < x.length(); i++)
        {
            if (x[i] == delim)
            {
                splitted.push_back(temp); // put split string into the output buff
                temp = "";
                i++;
            }
            temp += x[i];
        }
        return splitted;
    }

    public:
    std::map<std::string, std::string> argPairs; // Pairs of arguments, seperated by either ':' or '=' when supplied
    std::vector<std::string> arguments;
    bool Contains(std::string arg) {
        for (int i = 0; i < arguments.size(); i++) {
            if (arguments.at(i) == arg) { return true;}
        }
        return false;
    }
    /**
     * @brief returns the value directly after '='
     * @note The argument request does split it by '=', so you don't need to split it.
     * @param arg argument to find
     * @return <std::string> argV
     */
    std::string ArgPair(std::string arg) {
        auto found = argPairs.find(arg);
        if (argPairs.size() == 0 || found == argPairs.end()) {
            return "";
        }
        return found->second; // If found, return the Value of the K:V map. 
    }
    ProgramArguments(int argc, const char* argv[]) {
        for (int i = 0; i < argc; i++) {
            arguments.push_back(argv[i]);
        }
        for (int i = 0; i < arguments.size(); i++) {
            std::vector<std::string> test_colon = ALIB_PROG_ARG_split_string(arguments.at(i), ':');
            std::vector<std::string> test_eq    = ALIB_PROG_ARG_split_string(arguments.at(i), '=');
            if (test_colon.size() != 0) {
                argPairs.insert({test_colon[0], test_colon[1]});
            }
            if (test_eq.size() != 0) {
                argPairs.insert({test_eq[0], test_eq[1]});
            }
        }
    }
};
#endif // NDEF(ALIB_ARGPARSE_IMPL_ONCE)
#endif // NDEF(ALIB_NO_ARGPARSE)




#endif // __lib_aveth_utils_hppㅤ