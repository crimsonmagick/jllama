enum Compilers {

  CLANG("Unix Makefiles", null, "clang --version"),
  GCC("Unix Makefiles", null, 'gcc --version'),
  MINGW("MinGW Makefiles", "/src/windows-gcc-toolchain.cmake", "gcc --version"),
  MSVC("Visual Studio 17 2022", "/src/windows-msvc-toolchain.cmake", "cl"),
  NONE_FOUND(null, null, null)

  final generator
  final toolchain
  final checkCommand;

  Compilers(final String generator, final String toolchain, final String checkCommand) {
    this.generator = generator
    this.toolchain = toolchain
    this.checkCommand = checkCommand
  }

  boolean isPresent() {
    try {
      final output = checkCommand.execute()
      if (this == MINGW && !output.contains('MinGW')) {
        return false
      }
      return true
    } catch (final Exception ignored) {
      return false
    }
  }

}