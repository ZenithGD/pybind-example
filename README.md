# Pybind example

> **Warning** This has only been tested in Ubuntu! I'll update this repo soon to make it work for all platforms.

## About this repo

This is an example for setting up pybind as a git submodule in a CMake C++ project with multiple Python modules. Might not be the best way since I'm still learning CMake, but this is the best option I found, and it can easily be extended to larger projects. 

Even though this uses C++20 as the standard, it doesn't make use of the newly introduced `module` statements that replace the `#include` preprocessor directive, since there isn't really a clean way of doing that. For that reason, I stuck to the good ol' includes.

## Cloning this repo

In order to clone this repo, don't forget to include the `--recurse-submodules` option! Otherwise, the required Pybind11 submodule won't be loaded. If you have already cloned without this option, you must enter the following command from the root of the repo:

```
git submodule update --init
```

---

## Step by step guide

Imagine you want to make some Python bindings for a C++ project. First, let's assume we have the following codebase, containing two folders, `foo` and `bar`, each one with a C++ header and source. The directory structure would follow this schema:

```tree
.
└── src
    ├── bar
    │   ├── bar.cpp
    │   ├── bar.hpp
    ├── foo
    │   ├── foo.cpp
    │   ├── foo.hpp
    └── main.cpp
```

Let's use CMake as the build system in order to automate the construction of the required libraries. First, you must add the required `CMakeLists.txt` files in order to create the targets to compile the project. For example, the `src/foo/CMakeLists.txt` file could be something like this:

```cmake
# Create the library target
add_library(main_foo
    foo.cpp
)

# Include the top level directory
target_include_directories(main_foo PUBLIC ${PROJECT_SOURCE_DIR})
```

This tells CMake to create a library with one source file (you can add as many as you want) and include the `src` directory, so angled includes like `#include <foo/foo.hpp>`{:.c++} are available. The `bar` folder will essentially be the same, changing the target name and sources accordingly.

In order to compile the whole `src` project and make an executable with the `main.cpp` source as entrypoint, you have to add a `CMakeLists.txt` on that folder. This one is a bit more complicated, since we must create the executable and link the targets to that executable:

```cmake
# Create a project
project(pybind-example-exec VERSION 0.1.0 LANGUAGES C CXX)

# Try to find a Python installation.
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)

# add the executable
add_executable(main
    main.cpp
)

# add the subdirectories
add_subdirectory(foo)
add_subdirectory(bar)

# link the library targets against the executable target
target_link_libraries(main 
    PUBLIC main_foo
    PUBLIC main_bar)

# include the source folder
target_include_directories(main
  PUBLIC src)

# install binary and libs
install(TARGETS main DESTINATION bin)
install(TARGETS main_foo main_bar LIBRARY DESTINATION lib)
```

This CMake description does the following:

- It creates a project named `pybind-example-exec`.
- Tries to find a Python installation. For now we haven't used pybind11 yet, but this is necessary since otherwise we couldn't generate the bindings for our source code.
- Creates the executable target with `main.cpp` as an entrypoint.
- By calling `add_subdirectory`, CMake compiles each subfolder following its `CMakeLists.txt` description and exposes the library targets.
- It links the targets against the executable, so it can use the libraries from the subfolders.
- It installs the executable into a `dist/bin` folder, and the libraries into a `dist/lib` folder.

Finally, in order to compile the whole codebase including the external submodules (in this case, `pybind11`), we need to create the main `CMakeLists.txt` at the root of the project. This file will set the needed constants and add the `ext` subdirectory which contains the pybind11 distribution:

```cmake
# This is the main CMake build file
cmake_minimum_required (VERSION 3.16)
project(pybind-example LANGUAGES CXX VERSION 0.1)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")  # or 11, 14, 17, 20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # optional, ensure standard is supported
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # This avoids some annoying errors
SET(CMAKE_INSTALL_PREFIX dist) # Here you specify the directory in which files are installed.

# Add external dependencies
add_subdirectory(ext)

# Add the source code
add_subdirectory(src)
```

Finally, we must add a `CMakeLists.txt` file to the `ext` folder in order to add and compile pybind11. This one is pretty easy, since we just have to add the `pybind11` directory, because there's already a CMake file that does most of the work for us:

```cmake
# add the pybind11 submodule
add_subdirectory(pybind11)

# more submodules would go here
```

Your project structure should now look like this:
```
├── ext
│   ├── CMakeLists.txt
│   └─+ pybind11
└── src
    ├── CMakeLists.txt
    ├── __init__.py
    ├── bar
    │   ├── CMakeLists.txt
    │   ├── bar.cpp
    │   ├── bar.hpp
    ├── foo
    │   ├── CMakeLists.txt
    │   ├── foo.cpp
    │   ├── foo.hpp
    └── main.cpp
``` 

Finally, in order to create the python bindings, you can create a `python` folder inside of each subfolder inside `src`. For example, the `src/foo/python/foo_v.cpp` would have the module description that exposes an `add` function that adds two integers:

```cpp
#include <pybind11/pybind11.h>
#include <foo/foo.hpp>

namespace py = pybind11;

PYBIND11_MODULE(main_foo_py, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers");
}
```

Now, you must modify the `src/foo/CMakeLists.txt` file in order to create the python module:

```cmake
# Create the library target
add_library(main_foo
    foo.cpp
)

# Include the top level directory
target_include_directories(main_foo PUBLIC ${PROJECT_SOURCE_DIR})

# Create a new python module
pybind11_add_module (
    main_foo_py
    python/foo_v.cpp
)

# Link the C++ library against the python module 
target_link_libraries(main_foo_py PRIVATE main_foo)

# Include the top level directory too
target_include_directories(main_foo_py PUBLIC ${PROJECT_SOURCE_DIR})
```

> **Warning** It is very important to set the python module target name the same as in the source code. Otherwise, if you try to use your library, it will print a very misleading dynamic import error! Notice we used `main_foo_py` both in the binding source code and in the CMake description.

The `bar` folder CMake description will be very similar.

Now, imagine you want to name your package `pybindpy`. We would like to be able to do imports like this:

```python
# import parts explicitly from the module
from pybindpy import add, Printer

# import whole submodule and rename it
import pybindpy.main_foo_py as foo
```

In order to do that, we will setup an `__init__.py` file in the `src` folder that will be copied into the python module folder:

```python
# import aliases
from pybindpy.main_foo_py import *
from pybindpy.main_bar_py import * 
```

Finally, we must copy the libraries and the `__init__.py` file to the `dist/python/pybindpy` folder, so adding `dist/python` to your `PYTHONPATH` will allow you to import the module anywhere locally. For that we must modify the `src/CMakeLists.txt` file:

```cmake
project(pybind-example-exec VERSION 0.1.0 LANGUAGES C CXX)
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)

# add the executable
add_executable(main
    main.cpp
)

add_subdirectory(foo)
add_subdirectory(bar)

target_link_libraries(main 
    PUBLIC main_foo
    PUBLIC main_bar)

target_include_directories(main
  PUBLIC src)

# install module init file
install(FILES __init__.py DESTINATION python/pybindpy)

# install binary and libs
install(TARGETS main DESTINATION bin)
install(TARGETS main_foo main_bar LIBRARY DESTINATION lib)
install(TARGETS main_foo_py main_bar_py LIBRARY DESTINATION python/pybindpy)
```

Note that now we install the init script and the python libraries to `dist/python/pybindpy`. Your final project folder structure should be as follows:

```
├── ext
│   ├── CMakeLists.txt
│   └─+ pybind11
└── src
    ├── bar
    │   ├── CMakeLists.txt
    │   ├── bar.cpp
    │   ├── bar.hpp
    │   └── python
    │       └── bar_v.cpp
    ├── foo
    │   ├── CMakeLists.txt
    │   ├── foo.cpp
    │   ├── foo.hpp
    │   └── python
    │       └── foo_v.cpp
    ├── CMakeLists.txt
    ├── __init__.py
    └── main.cpp
```

In order to compile and install the project, you can execute the following script:

```bash
#!/bin/bash
# Build and install
mkdir -p build
cd build
if cmake -GNinja ..
then
    if ninja
    then
        ninja install
        foo_path="$(pwd)/build/dist/bin"
        echo "Executable at $foo_path, use foo_setpath.sh to make it available in PATH."
    else
        echo "Couldn't generate project!"
    fi
else
    echo "Check the build errors!"
fi
```

Finally, in order to set the `PYTHONPATH` environment, you can launch a `source` command with the following script:

```bash
#!/bin/bash

BUILD_DIR="$(pwd)/build"
DIST_DIR="$BUILD_DIR/dist"
export PATH="$PATH:$DIST_DIR/bin"

export PYTHONPATH="$DIST_DIR/python:$PYTHONPATH"
```

Now, you should be able to run a simple script such as this:
```python
from pybindpy import add, Printer
p = Printer("print int")
p.print_int(add(1,2))
```

I hope this tutorial is helpful, since there aren't many resources that cover this thoroughly. If you have any suggestions, please feel free to post an issue above.