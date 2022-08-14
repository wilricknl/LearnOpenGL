# LearnOpenGL

Cross-platform CMake follow along with [LearnOpenGL](https://learnopengl.com/).

### Table of Contents

- [LearnOpenGL](#learnopengl)
    - [Table of Contents](#table-of-contents)
- [Requirements](#requirements)
- [Linux](#linux)
    - [Install libraries](#install-libraries)
    - [CLion](#clion)
    - [Configure vcpkg in CLion](#configure-vcpkg-in-clion)
- [Windows](#windows)
    - [Install libraries](#install-libraries-1)
    - [Visual Studio 2022](#visual-studio-2022)
    - [Configure vcpkg in Visual Studio 2022](#configure-vcpkg-in-visual-studio-2022)

# Requirements

* [CLion](https://www.jetbrains.com/clion/): the IDE I use on Linux.
* [vcpkg](https://github.com/microsoft/vcpkg): a package manager to easily
  install required libraries.
* [Visual Studio 2022](https://visualstudio.microsoft.com/vs/): the IDE I use 
  on Windows. 

# Linux

### Install libraries

```shell
./vcpkg install glad glfw3 glm stb --triplet=x64-linux
```

### CLion

1. Clone this repository.
2. In CLion go to `File -> Open` and open the root `CMakeLists.txt` in this
   repository as a project.

### Configure vcpkg in CLion

1. Go to `File -> Settings -> Build, Execution, Deployment -> CMake`.
2. Find `CMake Options`.
3. Insert `-DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake`.

# Windows

### Install libraries

```shell
vcpkg.exe install glad glfw3 glm stb --triplet=x64-windows-static
```

### Visual Studio 2022

1. Clone this repository.
2. In Visual Studio go to `File -> Open -> Folder` and open this folder.
3. Once the folder is opened, right click on the root `CMakeLists.txt` and
   select `Set as Startup Item`.

### Configure vcpkg in Visual Studio 2022

Make sure `vcpkg` is system wide installed by running 
`vcpkg.exe integrate install`.

1. Go to `Project -> CMake Settings for LearnOpenGL`
2. Make sure `VCPKG_TARGET_TRIPLET` is set to `x64-windows-static`. By default
   the non-static triplet is used. The `CMakeLists.txt` is setup to compile
   statically, therefore it's necessary to update this option.
