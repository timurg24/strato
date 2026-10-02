# strato

Open source flight simulator powered by JSBSim

## Folder Structure

- `Engine` - Engine core (Wrangler)
- `Game` - Strato Flight Simulator code
- `Content` - Strato Flight Simulator content (including Game and Engine packages)
- `thirdparty` - Third party libraries

## Building

- Update/install submodules using `git submodule update --init --recursive`
- Generate your CMake project and compile as normal
- Compile `shaderc` using `cmake --build build --target shaderc` (if using developing shaders)
