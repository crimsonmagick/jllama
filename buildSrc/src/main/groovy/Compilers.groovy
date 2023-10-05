enum Compilers {

  CLANG("clang --version"),
  GCC('gcc --version'),
  MINGW("gcc --version"),
  MSVC("cl"),
  NONE_FOUND(null)

  final checkCommand

  Compilers(final String checkCommand) {
    this.checkCommand = checkCommand
  }

  boolean isPresent() {
    try {
      final process = checkCommand.execute()
      process.waitFor()
      final output = "$process.in.text"
      if (this == MINGW && !output.contains('MinGW')) {
        return false
      }
      return true
    } catch (final Exception ignored) {
      return false
    }
  }
}