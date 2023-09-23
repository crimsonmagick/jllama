enum Compilers {

  CLANG("Unix Makefiles", "clang --version"),
  GCC("Unix Makefiles",  'gcc --version'),
  MINGW("MinGW Makefiles", "gcc --version"),
  MSVC("Visual Studio 17 2022", "cl"),
  NONE_FOUND(null,  null)

  final generator
  final checkCommand

  Compilers(final String generator, final String checkCommand) {
    this.generator = generator
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