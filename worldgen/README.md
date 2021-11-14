## Instructions for Loading a World Generation File (.hkb)

We will try to parse a few standard commands to generate the world. There are different types of branches you can
create:
- 'b' for branch - 's' for stack - 'f' for flower

For option 'b' are different colors too:
- 'r' for red - 'g' for green - 'b' for blue

For stacks, these options work as well. But to create a stack that represent some number, use 'f'.

Afterwards, you must specify the coordinates in 3 dimensions as floats. For stacks, specify the root first, then specify
the vector kwargs.

The file will be converted into a set of nodes and edges that will be rendered into the world.

## Instructions for Using the Finite Random World Generator

Run the executable `finite` without arguments to see the options. If you just specify an output file path, it will
generate a random world with sensible defaults.