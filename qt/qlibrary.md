# qlibrary

When loading the library, QLibrary searches in all the system-specific library locations (e.g. LD_LIBRARY_PATH on Unix), unless the file name has an absolute path.

If the file name is an absolute path then an attempt is made to load this path first. If the file cannot be found, QLibrary tries the name with different platform-specific file prefixes, like "lib" on Unix and Mac, and suffixes, like ".so" on Unix, ".dylib" on the Mac, or ".dll" on Windows.

If the file path is not absolute then QLibrary modifies the search order to try the system-specific prefixes and suffixes first, followed by the file path specified.



# qlibrary 加载库或插件和解析符号,在linux平台下是通过dlopen, dlclose和dlsym函数实现的.

``` c++

// qlibrary_unix.cpp

bool QLibraryPrivate::load_sys()
{
...
    hnd = dlopen(QFile::encodeName(attempt), dlFlags);
...
}

QFunctionPointer QLibraryPrivate::resolve_sys(const char* symbol)
{
    QFunctionPointer address = QFunctionPointer(dlsym(pHnd.loadAcquire(), symbol));
    return address;
}
```

# dlopen

When you call dlopen() with a library name, it searches for the library file in a set of default locations, including:

- The directories listed in the LD_LIBRARY_PATH environment variable
- The system's default library directories (e.g., /usr/lib, /usr/local/lib)
- Any additional directories specified by configuration files such as /etc/ld.so.conf.d/*.conf