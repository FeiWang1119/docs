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
