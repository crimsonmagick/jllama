class Toolchains {

    private Toolchains() {}

    private static Map<String, String> toolChains = [:]

    static {
        toolChains[key(CompilerOs.LINUX, Architectures.x64, Compilers.GCC)] = 'linux-x64-gcc-toolchain.cmake'
        toolChains[key(CompilerOs.MACOS, Architectures.aarch64, Compilers.CLANG)] = 'mac-aarch64-clang-toolchain.cmake'
        toolChains[key(CompilerOs.WINDOWS, Architectures.x64, Compilers.MINGW)] = 'windows-gcc-toolchain.cmake'
        toolChains[key(CompilerOs.WINDOWS, Architectures.x64, Compilers.MSVC)] = 'windows-msvc-toolchain.cmake'
    }

    static def getToolchain(final CompilerOs compilerOs, final Architectures architecture, final Compilers compiler) {
        return toolChains[key(compilerOs, architecture, compiler)]

    }

    private static def key(final CompilerOs compilerOs, final Architectures architecture,
                    final Compilers compiler) {
        return "${compilerOs.name()}-${architecture.name()}-${compiler.name()}" as String
    }
}