# LearnOpenGL

Cross-platform follow along with [LearnOpenGL](https://learnopengl.com/).

### Table of Contents

- [LearnOpenGL](#learnopengl)
    - [Table of Contents](#table-of-contents)
- [Requirements](#requirements)
- [Linux](#linux)
    - [Install libraries](#install-libraries)
    - [CLion](#clion)
    - [Configure vcpkg in CLion](#configure-vcpkg-in-clion)

# Requirements

* [CLion](https://www.jetbrains.com/clion/): the IDE I use on Linux.
* [vcpkg](https://github.com/microsoft/vcpkg): a package manager to easily
  install required libraries.

# Linux

### Install libraries

```shell
./vcpkg install glad:x64-linux
./vcpkg install glfw3:x64-linux
```

### CLion

1. Clone this repository
2. In CLion go to `File -> Open` and open the root `CMakeLists.txt` as a 
   project.

### Configure vcpkg in CLion

1. Go to `File -> Settings -> Build, Execution, Deployment -> CMake`
2. Find `CMake Options`
3. Insert `-DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake`
