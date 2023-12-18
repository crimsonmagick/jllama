set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_C_COMPILER "/usr/bin/x86_64-w64-mingw32-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/x86_64-w64-mingw32-g++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads -static-libgcc -static-libstdc++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthreads -static-libgcc")

set(USING_MINGW_TOOLCHAIN TRUE)
