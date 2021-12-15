cmake_minimum_required(VERSION 3.0.0)
project(winmgr VERSION 0.1.0)

include(CTest)
enable_testing()


link_directories(${CMAKE_SOURCE_DIR}/lib)

link_directories("C:/Code/Lib/x64") # Delete this if you don't have this. It just has .lib or .o files for the below libraries



file(GLOB SRC_FILES
     "source/*.cc"
     "source/*.cpp"
     "source/*.cxx"
)

include_directories("C:/Code/Include")


add_executable(winmgr 
    "source/main.cpp" # Compile main.cpp first!

    ${SRC_FILES}
)

target_link_libraries(winmgr SDL2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
