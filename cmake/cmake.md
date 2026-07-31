# cmake 指定特定路经下的qt库(`CMAKE_PREFIX_PATH`)

``` sh
cmake -DCMAKE_PREFIX_PATH=$HOME/Qt/5.11.2/gcc_64 ..
```

## CMAKE_PREFIX_PATH

Semicolon-separated list of directories specifying installation prefixes to be searched by the find_package(), find_program(), find_library(), find_file(), and find_path() commands. Each command will add appropriate subdirectories (like bin, lib, or include) as specified in its own documentation.

- Make sure to clear the build directory before changing `CMAKE_PREFIX_PATH`
- Are you sure there's a lib/cmake folder inside /home/cavit/Qt/5.6? (That's where CMake finds the Qt5 config files)

# cmake build & install

``` sh
cmake -Bbuild -GNinja
cmake --build build
sudo cmake --install build

```

# Directory Paths

|Variable|Info|
|--|--|
|CMAKE_SOURCE_DIR |  The root source directory|
|CMAKE_CURRENT_SOURCE_DIR | The current source directory if using sub-projects and directories.|
|PROJECT_SOURCE_DIR | The source directory of the current cmake project.|
|CMAKE_BINARY_DIR |  The root binary / build directory. This is the directory where you ran the cmake command.|
|CMAKE_CURRENT_BINARY_DIR | The build directory you are currently in.|
|PROJECT_BINARY_DIR |  The build directory for the current project.|

# sub-projects

```cmake
add_subdirectory(sublibrary1)
add_subdirectory(sublibrary2)
add_subdirectory(subbinary)
```

## To reference the source directory for a different project you can use.

${sublibrary1_SOURCE_DIR}
${sublibrary2_SOURCE_DIR}

|Variable|Info|
|--|--|
|PROJECT_NAME | The name of the project set by the current project().|
|CMAKE_PROJECT_NAME | the name of the first project set by the project() command, i.e. the top level project.|
|PROJECT_SOURCE_DIR | The source directory of the current project.|
|PROJECT_BINARY_DIR | The build directory for the current project.|
|name_SOURCE_DIR | The source directory of the project called "name". sublibrary1_SOURCE_DIR|
|name_BINARY_DIR | The binary directory of the project called "name". sublibrary1_BINARY_DIR|

# Public VS Private VS Interface

https://leimao.github.io/blog/CMake-Public-Private-Interface/

## Include Inheritance

The `INCLUDE_DIRECTORIES` will be used for the current target only and the `INTERFACE_INCLUDE_DIRECTORIES` will be appended to the `INCLUDE_DIRECTORIES` of any other target which has dependencies on the current target. 

|Link Type|Description|
|--|--|
|PUBLIC | All the directories following PUBLIC will be used for the current target and the other targets that have dependencies on the current target, i.e., appending the directories to INCLUDE_DIRECTORIES and INTERFACE_INCLUDE_DIRECTORIES.|
|PRIVATE | All the include directories following PRIVATE will be used for the current target only, i.e., appending the directories to INCLUDE_DIRECTORIES.|
|INTERFACE | All the include directories following INTERFACE will NOT be used for the current target but will be accessible for the other targets that have dependencies on the current target, i.e., appending the directories to INTERFACE_INCLUDE_DIRECTORIES. |

## Link Inheritance

|Link Type|Description|
|--|--|
|PUBLIC | All the objects following PUBLIC will be used for linking to the current target and providing the interface to the other targets that have dependencies on the current target.|
|PRIVATE | All the objects following PRIVATE will only be used for linking to the current target.|
|INTERFACE | All the objects following INTERFACE will only be used for providing the interface to the other targets that have dependencies on the current target.|

## Conclusion

`PRIVATE` only cares about himself and does not allow inheritance. `INTERFACE` only cares about others and allows inheritance. `PUBLIC` cares about everyone and allows inheritance.

# find_package

find_package(<PackageName> [<version>] [REQUIRED] [COMPONENTS <components>...])

## Two Modes

Table 1: Key Differences Overview

| Feature | Module Mode | Config Mode |
| :--- | :--- | :--- |
| File searched | `Find<PackageName>.cmake` | `<PackageName>Config.cmake`<br>or `<lowercase-package>-config.cmake` |
| Provider | External (CMake, OS, or user) | The package itself |
| Primary search path | `CMAKE_MODULE_PATH` | `CMAKE_PREFIX_PATH` |
| Version matching | Limited / heuristic | ✅ Full support (`EXACT`, `VERSION`) |
| Component support | Limited / manual | ✅ Native (`COMPONENTS`, `OPTIONAL_COMPONENTS`) |
| Transitive dependencies | Manual handling | ✅ Automatic propagation |
| Target quality | Generic imported targets | Precise targets with full metadata |
| Reliability | ⚠️ May be outdated | ✅ Always matches installed package |
| Control preference | `MODULE` option | `CONFIG` or `NO_MODULE` option |

Table 2: Search Path Comparison

| Search Source | Module Mode | Config Mode |
| :--- | :--- | :--- |
| User-specified (`PATHS`/`HINTS`) | ✅ Yes | ✅ Yes |
| `CMAKE_MODULE_PATH` | ✅ **Primary** | ❌ No |
| `CMAKE_PREFIX_PATH` | ❌ No | ✅ **Primary** |
| `<PackageName>_DIR` | ❌ No | ✅ Yes |
| System paths (e.g., `/usr`, `/usr/local`) | ✅ Yes | ✅ Yes |
| `PATH` environment variable | ✅ Yes | ✅ Yes |

## Search Priority

Table 1: Module Mode Search Priority

| Priority | Search Location | Description |
| :---: | :--- | :--- |
| 1 | User-provided `PATHS` / `HINTS` | Explicit paths passed to `find_package` |
| 2 | `CMAKE_MODULE_PATH` | User-specified directories (highest priority) |
| 3 | CMake's built-in module directory | CMake installation's `Modules/` folder |

Table 2: Config Mode Search Priority (Detailed)

| Priority | Search Location | CMake Variable / Source |
| :---: | :--- | :--- |
| 1 | Explicit paths from `PATHS` / `HINTS` | Direct arguments to `find_package` |
| 2 | User CMake variable | `<PackageName>_DIR` (cached) |
| 3 | User CMake path variables | `CMAKE_PREFIX_PATH`, `CMAKE_FRAMEWORK_PATH`, `CMAKE_APPBUNDLE_PATH` |
| 4 | User environment variables | `<PackageName>_DIR` (env) |
| 5 | System environment variables | `CMAKE_PREFIX_PATH`, `CMAKE_FRAMEWORK_PATH`, `CMAKE_APPBUNDLE_PATH` (env) |
| 6 | `PATH` environment variable | System `PATH` (derived directories) |
| 7 | System default paths | `/usr`, `/usr/local`, `/opt`, registry keys (Windows) |