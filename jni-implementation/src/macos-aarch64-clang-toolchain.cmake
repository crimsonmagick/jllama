set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR "aarch64")

set(CMAKE_C_COMPILER "/path/to/aarch64-linux-gnu-clang")
set(CMAKE_CXX_COMPILER "/path/to/aarch64-linux-gnu-clang++")

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

# Add any compiler flags you might need
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target aarch64-apple-darwin")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target aarch64-apple-darwin")

# Optionally set the sysroot if required (you need to specify the correct path)
# set(CMAKE_OSX_SYSROOT "/path/to/MacOSX.sdk")

# Any other settings you might need
