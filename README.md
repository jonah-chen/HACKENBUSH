# HACKENBUSH

Inspired by Owen Maitzen's video, this is my implementation of the HACKENBUSH game.

[![Inspiring video about the game](http://i3.ytimg.com/vi/ZYj4NkeGPdM/maxresdefault.jpg)](https://youtu.be/ZYj4NkeGPdM)

### Video Description

A playful venture into the vast and mysterious forests of combinatorial game theory.

This one simple game will change the way you look at numbers forever! Hackenbush is easy to pick up, but exploring its
strategy leads us down a fantastical mathematical rabbit hole, through which can be found the roots of our familiar
number system, intricately entangled with a host of new and exotic numeric species lurking alongside them.

### Build:

- Make sure the prerequisites are installed. I am compiling with OpenGL4.6 and C++20 using cmake (make, clang, glm, GLFW
  are required too).
- Generate build configuration using `cmake .`
- Build with `cd build && make`.
- Execute `./HACKENBUSH` with the `--help` flag to see the available options.

### Features:

- OpenGL environment allowing Minecraft-like movement (without gravity) and a ground.
- Rendering of nodes and edges in 3D space.
- Infinite world.
- Finite collections of red, blue, or green branches.
- Infinite stacks of branches (only the harmonic generator is implemented for now).
- Player can chop down branches, and unsupported branches will disappear.
- Basic world generator (see `testworld.hkb`).

### TODO:

- Menu screen
- Ground texture
- Procedural world generation

### More Ambitious Goals:

- Online multiplayer game
- World editor mode
- More different branch types & generators
- Python wrapper?
