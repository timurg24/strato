# Starto FS To-do list

A reminder of some features I might forget about

## Features

- [ ] Weather
- [ ] Turbulence

## Technical

- [x] Replace `.pak` and `Filesystem` with normal folders to match JSBSim's requirement for physical folders. There should now be no difference between Engine and Game content, making the file structure look more neat.
- [ ] Add Dear ImGui
- [ ] Add RmlUI
- [ ] Actually do something with payload lbs, or replace it with a more flexible system for multiple load points
- [ ] Replace Content folder with a Chisel-style "Working" folder
  - Working folder contains source files (shader sources, textures, payloads, etc.)
  - "Cooked" folder contains finished files (shader binaries, optimized textures, etc.)
