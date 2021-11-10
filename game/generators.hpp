#pragma once

#include "nodes.hpp"
#include <cmath>

#define DEFAULT_GEOMETRIC_HEIGHT 1.0f

namespace game { namespace nodes{ namespace generators {

namespace F {
    branch_type red  (const int64_t order, void* kwargs);
    branch_type green(const int64_t order, void* kwargs);
    branch_type blue (const int64_t order, void* kwargs);
    branch_type num  (const int64_t order, void* kwargs);
}

namespace f {
    glm::vec3 linear        (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
    glm::vec3 harmonic      (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
    glm::vec3 quadratic     (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
    glm::vec3 geometric     (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
    glm::vec3 c_quadratic   (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
    glm::vec3 c_geometric   (const int64_t depth, const glm::vec3 &pos, 
                             const glm::vec3 &kwargs);
}

namespace f_{
    int64_t linear     (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t harmonic   (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t quadratic  (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t harmonic   (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t geometric  (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t c_quadratic(const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs);
    int64_t c_geometric(const glm::vec3 &bottomleft, const glm::vec3 &topright,
                        const glm::vec3 &rootpos,    const glm::vec3 &kwargs); 
}

using type_gen = branch_type (*)(const int64_t, void*);

type_gen ALL_RED;
type_gen ALL_GREEN;
type_gen ALL_BLUE;

struct step_gen {
    glm::vec3   (*a) (const    int64_t, const glm::vec3&, const glm::vec3&);
    int64_t     (*a_)(const glm::vec3&, const glm::vec3&, const glm::vec3&, 
                      const glm::vec3&);
};

step_gen LINEAR;
step_gen HARMONIC;
step_gen QUADRATIC;
step_gen GEOMETRIC;

step_gen CIRCLE_QUADRATIC;
step_gen CIRCLE_GEOMETRIC;
}}}

