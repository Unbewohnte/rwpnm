cmake_minimum_required(VERSION 3.10)

project(rwpnm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(MSVC)
    # to be made, probably...
else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O2")
endif()

set(EXECUTABLE_OUTPUT_PATH ../bin)

FILE(GLOB source_files ../src/*)
add_executable(rwpnm ${source_files})
