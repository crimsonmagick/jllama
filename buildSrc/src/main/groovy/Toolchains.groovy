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
        toolChains[key(OperatingSystem.MAC_OS, OperatingSystem.LINUX, Architectures.arm64, Compilers.CLANG)] = new ToolChain('macos-linux-arm64-clang-toolchain.cmake', 'Unix Makefiles', ['JAVA_HOME': getMacJdkHome()])
        toolChains[key(OperatingSystem.MAC_OS, OperatingSystem.MAC_OS, Architectures.arm64, Compilers.CLANG)] = new ToolChain('macos-arm64-clang-toolchain.cmake', 'Unix Makefiles')
        toolChains[key(OperatingSystem.MAC_OS, OperatingSystem.MAC_OS, Architectures.arm64, Compilers.GCC)] = new ToolChain('macos-arm64-gcc-toolchain.cmake', 'Unix Makefiles')
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.LINUX, Architectures.x64, Compilers.MINGW)] = new ToolChain('windows-linux-mingw-toolchain.cmake', 'Unix Makefiles', ['JAVA_HOME': getWinJdkHome()])
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.WINDOWS, Architectures.x64, Compilers.MINGW)] = new ToolChain('windows-mingw-toolchain.cmake', 'MinGW Makefiles')
        toolChains[key(OperatingSystem.WINDOWS, OperatingSystem.WINDOWS, Architectures.x64, Compilers.MSVC)] = new ToolChain(null, 'Visual Studio 17 2022')
    }

    static def getToolchain(final OperatingSystem targetOs, OperatingSystem hostOs, final Architectures architecture, final Compilers compiler) {
        return toolChains[key(targetOs, hostOs, architecture, compiler)]
    }

    private static def key(final OperatingSystem targetOs, final OperatingSystem hostOs, final Architectures architecture, Compilers compiler) {
        return "${targetOs.getFamilyName()}-${hostOs.getFamilyName()}-${architecture.name()}-${compiler.name()}}" as String
    }

    private static def getMacJdkHome() {
        final MAC_JAVA_HOME = 'JDK_MAC_LOCATION'
        final location = System.hasProperty(MAC_JAVA_HOME) ? System.getProperty(MAC_JAVA_HOME) : System.getenv(MAC_JAVA_HOME) ? System.getenv(MAC_JAVA_HOME) : '/usr/local/include/jdk-17-mac'
        return "${location}/Contents/Home";
    }

    private static def getWinJdkHome() {
        final WIN_JAVA_HOME = 'JDK_WIN_LOCATION'
        return System.hasProperty(WIN_JAVA_HOME) ? System.getProperty(WIN_JAVA_HOME) : System.getenv(WIN_JAVA_HOME) ? System.getenv(WIN_JAVA_HOME) : '/usr/local/include/jdk-17-win'
    }
}