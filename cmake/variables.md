# CMake Build System Variables

## CMAKE_BINARY_DIR
Top-level build directory (where cmake is run).

## CMAKE_SOURCE_DIR
Top-level source directory (where the root CMakeLists.txt is).

## CMAKE_CURRENT_BINARY_DIR
Current build directory being processed.

## CMAKE_CURRENT_SOURCE_DIR
Current source directory being processed.

## CMAKE_CURRENT_LIST_DIR
Directory of the current CMake file being processed.

## CMAKE_CURRENT_LIST_FILE
Full path to the current CMake file being processed.

## CMAKE_MODULE_PATH
List of directories to search for CMake modules.

## CMAKE_COMMAND
Full path to the cmake executable.

# Project Information Variables

## PROJECT_NAME
Name of the current project.

## PROJECT_SOURCE_DIR
Source directory of the current project.

## PROJECT_BINARY_DIR
Build directory of the current project.

## CMAKE_PROJECT_NAME
Name of the top-level project.

# Build Configuration Variables

## CMAKE_BUILD_TYPE
Build type (Debug, Release, RelWithDebInfo, MinSizeRel).

## CMAKE_CONFIGURATION_TYPES
Available build configurations (multi-config generators like Visual Studio).

## CMAKE_DEBUG_POSTFIX
Suffix for debug libraries (e.g., _d).

## CMAKE_RELEASE_POSTFIX
Suffix for release libraries.

# Compiler and Toolchain Variables

## CMAKE_C_COMPILER
Path to the C compiler.

## CMAKE_CXX_COMPILER
Path to the C++ compiler.

## CMAKE_C_COMPILER_ID
Compiler ID (e.g., GNU, Clang, MSVC).

## CMAKE_CXX_COMPILER_ID
C++ compiler ID.

## CMAKE_C_FLAGS
Flags for the C compiler.

## CMAKE_CXX_FLAGS
Flags for the C++ compiler.

## CMAKE_EXE_LINKER_FLAGS
Linker flags for executables.

## CMAKE_SHARED_LINKER_FLAGS
Linker flags for shared libraries.

## CMAKE_STATIC_LINKER_FLAGS
Linker flags for static libraries.

## CMAKE_SYSTEM_NAME
Target system name (e.g., Linux, Windows, Darwin).

## CMAKE_SYSTEM_PROCESSOR
Target processor (e.g., x86_64, ARM).

# Installation Variables

## CMAKE_INSTALL_PREFIX
Installation prefix (default: /usr/local on Unix, C:\Program Files on Windows).

## CMAKE_INSTALL_BINDIR
Binary install directory (default: bin).

## CMAKE_INSTALL_LIBDIR
Library install directory (default: lib).

## CMAKE_INSTALL_INCLUDEDIR
Include file install directory