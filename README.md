# strato (Filament Branch)

Developing branch to switch the renderer to Google Filament

## Folder Structure

- `Engine` - Engine core (Wrangler)
- `Game` - Strato Flight Simulator code
- `Content` - Strato and Aircraft source content (like the `Working` directory in Chisel)
- `thirdparty` - Third party libraries

## Requirements

- C++20 compiler
- Python

## Building

- Update/install submodules using `git submodule update --init --recursive`
- Generate your CMake project and compile as normal
- Each time you compile, the content from the `Content` folder is copied to the build directory
