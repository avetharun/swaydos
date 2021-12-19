/**
 * @file swayconfig.hpp
 * @author Avetharun (avetharun@lazyne.co)
 * @brief Config file importer for window manager
 * @version 0.1
 * @date 2021-12-14

 @todo: this file... Need to get the basics done, but I want to emulate SwayWM's config as closely as possible.
 
 
  */
#include <SDL2/SDL_keycode.h>
#include <vector>
#include <map>
#include <algorithm>
#include <Shlobj.h>


const char* getHomeDir() {
    char path[ MAX_PATH ];
    if (SHGetFolderPathA( NULL, CSIDL_PROFILE, NULL, 0, path ) != S_OK) {
      return nullptr;
    }
    return reinterpret_cast<const char*>(path);
}
struct GFConfig{
    std::map<SDL_KeyCode, char*> keyMapFuncs; // Mapped functions/executables to keys
};
int checkSwayConfig() {
    std::string swaydos_config_f = std::string(getHomeDir())+std::string("/.swaydos/config");
    std::replace(swaydos_config_f.begin(), swaydos_config_f.end(), '\\', '/'); // Replace all backslashes with forward slashes. Since windows is mean.
    if ( file_exists(swaydos_config_f.c_str()) ) {
        
    }
    return 0;
}