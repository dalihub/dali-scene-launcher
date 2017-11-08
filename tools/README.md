<img src="https://dalihub.github.io/images/DaliLogo320x200.png">

# Table of Contents

  * [Tools Contents](#tools-contents)
  * [Build Instructions](#build-instructions)
    * [1. Minimum Requirements](#1-minimum-requirements)
    * [2. Building with Eclipse](#2-building-with-eclipse)
    * [3. Building with CodeBlocks](#3-building-with-codeblocks)

# Tools Contents

  * **assimp-4.0.1**: Open Asset Importer headers and prebuilt libraries required by model-exporter-core. More info: https://github.com/assimp/assimp

  * **model-exporter-core**: static library that implements the conversion of Collada scenes (.dae) to DALi scenes (.dli and .bin files). Eclipse and Codeblocks projects are provided, for Windows and Linux.

  * **model-exporter-cli**: command line exporter built on model-exporter-core. Codeblocks project is provided, for Windows and Linux.

  * **model-exporter-jni**: shared library that exposes model-exporter-core's functionality to Java with a few minor additions. Eclipse and Codeblocks projects are provided, for Windows and Linux.

  * **com.samsung.dali.modelConverter**: Eclipse Rich Client Platform application project which provides a GUI to model-exporter-core. Eclipse project is provided for both platforms.

# Build Instructions

## 1. Minimum Requirements

 * Windows: Windows 8 or later
   * MinGW32
   * 32-bit JDK v1.8
   * 32-bit Eclipse v4.7.1. RCP & RAP
   * CodeBlocks > 13.12

   WARNING: the linking of model-exporter-jni is known to be problematic on Windows with GCC latest at the time of writing (6.3.0).

 * Linux: Ubuntu 14.04 or later.
   * GCC >= v4.9.0
   * 64-bit JDK v1.8
   * 64-bit Eclipse v4.7.1. RCP & RAP
   * CodeBlocks > 13.12

 * Both platforms: install the' C++ Development Tools' for Eclipse. ('Help' -> 'Install New Software...')

 * modelConverter offers a few tools - currently in development - that facilitate the use of the Smart Development Bridge that comes with [Tizen Studio](https://developer.tizen.org/development/tizen-studio/download). To be able to use these, make sure that SDB is in your PATH.

## 2. Building with Eclipse

This folder may be used as an Eclipse workspace from which the projects can be imported as "Existing Projects (into Workspace)".

You might be required to set the JAVA_HOME / JDK_HOME environment variables to the location of your JDK installation (i.e. C:\Program Files (x86)\Java\jdk1.8.0_152 or /usr/lib/jvm/java-8-oracle).

You might want to make sure that you're building the correct configuration (right-click project -> 'Build Configurations' -> 'Set Active'); Eclipse will default to the first target, which is Linux, and this might lead to misleading build errors on Windows.

To launch com.samsung.dali.modelConverter for the first time, open the similarly named **.product** file, and choose "Launch as Eclipse Application".

## 3. Building with Codeblocks

The 'model-exporter.workspace' is provided which contains all three model-exporter projects with Windows (32-bit) and Linux (64-bit) targets.

