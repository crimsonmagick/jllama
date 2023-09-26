import org.gradle.internal.os.OperatingSystem

class Toolchains {

    static class ToolChain {

        ToolChain(final String file, final String generator) {
            this(file, generator, null)
        }

        ToolChain(final String file, final String generator, final Map<String, String> customDefs) {
            this.file = file
            this.generator = generator
            this.customDefs = customDefs ? customDefs : [:]
        }

        String file
        String generator
        Map<String, String> customDefs
    }

    private Toolchains() {}

    private static Map<String, ToolChain> toolChains = [:]

    static {
        toolChains[key(OperatingSystem.LINUX, OperatingSystem.LINUX, Architectures.x64, Compilers.GCC)] = new ToolChain('linux-x64-gcc-toolchain.cmake', 'Unix Makefiles')
        toolChains[key(OperatingSystem.MAC_OS, OperatingSystem.LINUX, Architectures.aarch64, Compilers.CLANG)] = new ToolChain('MAC_OS-linux-aarch64-clang-toolchain.cmake', 'Unix Makefiles')
        toolChains[key(OperatingSystem.MAC_OS, OperatingSystem.MAC_OS, Architectures.aarch64, Compilers.CLANG)] = new ToolChain('MAC_OS-aarch64-clang-toolchain.cmake', 'Unix Makefiles')
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.LINUX, Architectures.x64, Compilers.MINGW)] = new ToolChain('windows-linux-mingw-toolchain.cmake', 'Unix Makefiles', ['JAVA_HOME': getWinJdkHome()])
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.WINDOWS, Architectures.x64, Compilers.MINGW)] = new ToolChain('windows-mingw-toolchain.cmake', 'MinGW Makefiles')
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.WINDOWS, Architectures.x64, Compilers.MSVC)] = new ToolChain('windows-msvc-toolchain.cmake', 'Visual Studio 17 2022')
    }

    static def getToolchain(final OperatingSystem targetOs, OperatingSystem hostOs, final Architectures architecture, final Compilers compiler) {
        return toolChains[key(targetOs, hostOs, architecture, compiler)]
    }

    private static def key(final OperatingSystem targetOs, final OperatingSystem hostOs, final Architectures architecture, Compilers compiler) {
        return "${targetOs.getFamilyName()}-${hostOs.getFamilyName()}-${architecture.name()}-${compiler.name()}}" as String
    }

    private static def getWinJdkHome() {
        final WIN_JAVA_HOME = 'WIN_JAVA_HOME'
        return System.hasProperty(WIN_JAVA_HOME) ? System.getProperty(WIN_JAVA_HOME) : System.getenv(WIN_JAVA_HOME) ? System.getenv(WIN_JAVA_HOME) : '/usr/local/include/jdk-17-win/jdk-17.0.8.1+1'
    }
}