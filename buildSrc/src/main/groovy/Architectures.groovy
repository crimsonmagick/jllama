enum Architectures {
  x64, arm64, OTHER

  static Architectures getHostArch() {
    def arch = System.getProperty("os.arch")
    if (arch == "amd64" || arch == "x86_64") {
      return x64
    } else if (arch == "arm64") {
      return arm64
    } else {
      return OTHER
    }
  }

}