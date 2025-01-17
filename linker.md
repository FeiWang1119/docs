# default search path  

The linker ld uses a specific set of directories to find shared libraries on Linux systems. These directories are known as the "default search paths" and they include:

- /lib
- /usr/lib
- Directories listed in the /etc/ld.so.conf configuration file
- Directories listed in files under the /etc/ld.so.conf.d directory
- Additionally, the LD_LIBRARY_PATH environment variable can be used to specify additional directories where shared libraries can be found.
  
It's important to note that modifying the default search paths for shared libraries can have unintended consequences and should generally be avoided unless you have a specific reason to do so.