enum Compilers {

  CLANG("Unix Makefiles", null),
  GCC("Unix Makefiles", null),
  MINGW("MinGW Makefiles", "/src/windows-gcc-toolchain.cmake"),
  MSVC("Visual Studio 17 2022", "/src/windows-msvc-toolchain.cmake"),
  NONE_FOUND(null, null)

  final generator;
  final toolchain

  Compilers(final String generator, final String toolchain) {
    this.generator = generator;
    this.toolchain = toolchain
  }

}