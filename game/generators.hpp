#pragma once
#define LINEAR      (game::nodes::generators::step_gen){game::nodes::generators::f::linear, game::nodes::generators::f_::linear}
#define HARMONIC    (game::nodes::generators::step_gen){game::nodes::generators::f::harmonic, game::nodes::generators::f_::harmonic}
#define QUADRATIC   (game::nodes::generators::step_gen){game::nodes::generators::f::quadratic, game::nodes::generators::f_::quadratic}
#define GEOMETRIC   (game::nodes::generators::step_gen){game::nodes::generators::f::geometric, game::nodes::generators::f_::geometric}

#define CIRCLE_QUADRATIC (game::nodes::generators::step_gen){game::nodes::generators::f::circle_quadratic, game::nodes::generators::f_::circle_quadratic}
#define CIRCLE_GEOMETRIC (game::nodes::generators::step_gen){game::nodes::generators::f::circle_harmonic, game::nodes::generators::f_::circle_harmonic}