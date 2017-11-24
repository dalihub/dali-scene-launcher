package com.samsung.dali.modelconverter.controller;

/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.file.CopyOption;
import java.nio.file.Files;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import org.eclipse.core.runtime.FileLocator;
import org.eclipse.core.runtime.Platform;
import org.osgi.framework.Bundle;

public class FileUtils {

  public static File getBundleFile(String path) throws URISyntaxException, IOException
  {
    Bundle bundle = Platform.getBundle("com.samsung.dali.modelconverter");
    URL url = bundle.getEntry(path);
    File entry = new File(FileLocator.resolve(url).toURI());

    return entry;
  }

  public static void copyFolder(File source, File target, CopyOption... options) throws IOException
  {
    if (!target.exists()) {
      target.mkdirs();
    }

    File[] contents = source.listFiles();
    if (contents != null) {
      for (File f : contents) {
          File newFile = new File(target.getAbsolutePath() + File.separator + f.getName());
          if (f.isDirectory())
              copyFolder(f, newFile, options);
          else
              copyFile(f, newFile, options);
      }
    }
  }

  public static void copyFile(File source, File target, CopyOption... options) throws IOException {
    Files.copy(source.toPath(), target.toPath(), options);
  }

  public static void extractZip(File source, File target) throws IOException
  {
    if (!target.exists()) {
      target.mkdirs();
    }

    String targetPath = target.getAbsolutePath();
    if(!targetPath.endsWith(File.separator)) {
      targetPath += File.separator;
    }

    FileInputStream fileStream = new FileInputStream(source);
    try(ZipInputStream zipStream = new ZipInputStream(fileStream)) {

      byte[] buffer = new byte[1024];
      ZipEntry zipEntry;
      while ((zipEntry = zipStream.getNextEntry()) != null) {

        if(zipEntry.isDirectory()) {
          new File(targetPath + zipEntry.getName()).mkdirs();
        }
        else {
          String outPath = targetPath + zipEntry.getName();
          File outFile = new File(outPath);

          // This might be unnecessary, but we don't want to bet on ZipInputStream giving us entries depth-first.
          File parent = outFile.getParentFile();
          if(!parent.exists()) {
            parent.mkdirs();
          }

          try (FileOutputStream outStream = new FileOutputStream(outFile)) {
            int len;
            while ((len = zipStream.read(buffer)) > 0) {
              outStream.write(buffer, 0, len);
            }
          }
        }
      }
    }
  }
}
