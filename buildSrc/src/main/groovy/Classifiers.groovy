import org.gradle.internal.os.OperatingSystem

class Classifiers {
  static getClassifier(OperatingSystem os, Architectures arch) {
    return "${os.getFamilyName()}_${arch.name()}".replace(' ', '_')
  }

  static hostClassifier() {
    final os = OperatingSystem.current()
    final arch = Architectures.getHostArch()
    return getClassifier(os, arch)
  }
}
