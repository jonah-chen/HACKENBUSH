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
- World Parser (see `testworld.hkb`).
- Random (finite only) World Generator.

### Future Goals:

- Complete documentation
- Menu screen
- Ground texture
- Lighting
- Online multiplayer game
- World editor mode
- More different branch types & generators
- Python wrapper?

### Some Screenshots

#### Zero Position
<img width="960" alt="empty" src="https://user-images.githubusercontent.com/71615343/141711602-3b881066-970f-4919-b8ba-3ebb5a33b117.png">

#### Up
<img width="960" alt="up" src="https://user-images.githubusercontent.com/71615343/141711586-8a3ed07c-25d3-429e-a0cf-6c70e176ca09.png">


#### Double Omega + Negative Omega + Up
<img width="960" alt="2omega-omega+up" src="https://user-images.githubusercontent.com/71615343/141711567-5c79e655-fd1b-4a43-8b5d-f37030f86e1e.png">

#### Randomly generated world
<img width="960" alt="rng" src="https://user-images.githubusercontent.com/71615343/141711470-7648b10a-fa5c-470b-97ef-a7fdae775bb7.png">

### Comments from Me:

It has been great to work on this game. I love the concept, and the simplicity of the game but it has the potential to
be so complex. Implementing the game in software is a great learning experience but very challenging. It has been very
difficult to wrap my head around the infinities the game can create which I have yet to fully implement, I am happy with
being able to get the infinite stack to work today. Also, I know the graphics don't look the best, but this is my first
working graphical program. OpenGL is very challenging, but I finally got the basics down enough to have it working. Fall
break is over, so I would definitely not be able to have as much energy to work on this in the future.

### Finally

Happy belated birthday my good friend (not mentioning your name to everyone)! Sorry for the still incomplete project,
but I tried to wrap it up as much as I can in these few days.
