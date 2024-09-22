cmake_minimum_required(VERSION 3.22)

macro (dbg VARIABLE)
    message("${VARIABLE}: ${${VARIABLE}}")
endmacro (dbg VARIABLE)

#include(ExternalProject)
#include(GNUInstallDirs)

project(getopt-plus LANGUAGES C)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (WIN32)
    set(ENV{HOME} "$ENV{USERPROFILE}")
endif (WIN32)

if (WIN32)
    set(ADDITIONAL_FLAGS WIN32)
else (WIN32)
    set(ADDITIONAL_FLAGS)
endif (WIN32)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wconversion")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g0 -O3 -march=native -mtune=native -ftree-vectorize -funroll-loops -flto")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb3")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map=${PROJECT_NAME}.map")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,-O2,--sort-common,--as-needed,-z,relro,-z,now,--copy-dt-needed-entries,-flto")
    #target_link_libraries(${PROJECT_NAME} stdc++fs)
    add_definitions(-D__GNU_GCC__)
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Intel")
    # using Intel C++
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MAP:${PROJECT_NAME}.map")
endif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "${CMAKE_C_FLAGS_DEBUG_INIT}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_C_FLAGS_RELEASE_INIT}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_MINSIZEREL_INIT "${CMAKE_C_FLAGS_MINSIZEREL_INIT}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "${CMAKE_C_FLAGS_RELWITHDEBINFO_INIT}")


add_executable(${PROJECT_NAME}-test main.c)

add_subdirectory(getopt-plus)
target_link_libraries(${PROJECT_NAME}-test getopt-plus)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)

set(SRC_IDE .clang-format README.md)

add_custom_target(target_for_ide SOURCES ${SRC_IDE})

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}-test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
