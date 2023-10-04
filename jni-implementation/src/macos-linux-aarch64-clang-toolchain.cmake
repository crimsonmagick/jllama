set(CMAKE_SYSTEM_NAME Darwin)
set(CMAKE_SYSTEM_PROCESSOR "arm64e")

set(CMAKE_C_COMPILER "/usr/local/osxcross/bin/oa64e-clang")
set(CMAKE_CXX_COMPILER "/usr/local/osxcross/bin/oa64e-clang++")

set(ARM64_LINKER "/usr/local/osxcross/bin/arm64e-apple-darwin22.4-ld")
set(MAC_FRAMEWORKS "/usr/local/osxcross/SDK/MacOSX13.3.sdk/System/Library/Frameworks")
set(MAC_TARGET "arm64e-apple-darwin")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target ${MAC_TARGET} -fuse-ld=${ARM64_LINKER} -F${MAC_FRAMEWORKS}")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -target ${MAC_TARGET} -fuse-ld=${ARM64_LINKER} -F${MAC_FRAMEWORKS}")