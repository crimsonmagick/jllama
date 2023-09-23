enum Architectures {
  x64, aarch64, OTHER

  static Architectures getHostArch() {
    def arch = System.getProperty("os.arch")
    if (arch == "amd64" || arch == "x86_64") {
      return x64
    } else if (arch == "aarch64") {
      return aarch64
    } else {
      return OTHER
    }
  }

}