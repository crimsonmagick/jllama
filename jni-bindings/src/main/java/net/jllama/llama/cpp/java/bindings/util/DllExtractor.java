package net.jllama.llama.cpp.java.bindings.util;

import net.jllama.llama.cpp.java.bindings.exceptions.ResourceNotFoundException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Enumeration;

import static java.nio.file.Files.newOutputStream;

public class DllExtractor {

  final static String ERROR_MESSAGE = "Unable to find resource with resourceName=";

  public static String extract(final String libraryName) {
    final String libraryFileName = System.mapLibraryName(libraryName);
    try {
      final Path outputPath = Files.createTempDirectory("nativelibs").resolve(libraryFileName);
      final Enumeration<URL> dllEnumeration = Thread.currentThread().getContextClassLoader()
        .getResources(libraryFileName);
      if (dllEnumeration.hasMoreElements()) {
        final URL url = dllEnumeration.nextElement();
        try (
          final InputStream in = url.openStream();
          final OutputStream out = newOutputStream(outputPath)
        ) {
          final byte[] buffer = new byte[1024];
          int length;
          while ((length = in.read(buffer)) > 0) {
            out.write(buffer, 0, length);
          }
          return outputPath.toString();
        }
      }
    } catch (final IOException e) {
      throw new ResourceNotFoundException(ERROR_MESSAGE + libraryFileName, e);
    }
    throw new ResourceNotFoundException(ERROR_MESSAGE + libraryFileName);
  }
}
