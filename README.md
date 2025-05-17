# Fenng

Fenng, a multimedia engine and library.

# Compiling

Follow the normal CMake compilation steps, with the addition of defining `BOOST_ROOT` to where your Boost installation is. Set CMAKE_BUILD_TYPE to Release or Debug depending on whether you want debug symbols or not. Use `-DCMAKE_CXX_FLAGS=-DBOOST_ALL_NO_LIB=ON` if you do not want to build Boost separatedly. Remember that you can use `-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded` and `-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug` for compiling with a static C++ runtime on Windows and to match the value of `CMAKE_MSVC_RUNTIME_LIBRARY` with the build type (`CMAKE_BUILD_TYPE`): you should use `-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug` together with `-DCMAKE_BUILD_TYPE=Debug`, for instance.

- Example 1 (starting inside the project's root, cd-ing into the build directory):
```
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=<your/boost/path>
cmake --build .
```

- Example 2 (inside the project's root, specifying the generator, compiler, build type and the MSVC runtime (on Windows)):
```
mkdir build

cmake -S. -Bbuild -GNinja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebug -DBOOST_ROOT=<your/boost/path>

cmake --build build
```

# Credits

- GLAD (https://glad.dav1d.de): OpenGL initialisation;
- GLFW (https://github.com/glfw/glfw): window management and input handling;
- GLM (https://github.com/g-truc/glm): GLSL-like vector and matrix arithmetics;
- STB (https://github.com/nothings/stb): image-loading and image-writing;
- Boost (https://www.boost.org): general-purpose libraries;
- whereami (https://github.com/gpakosz/whereami): finding the location of the current executable;