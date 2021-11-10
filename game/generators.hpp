#pragma once

#include "nodes.hpp"
#include <cmath>

// define the rate geometric series shrinks
#define GEOMETRIC_CONSTANT 0.9f

namespace game { namespace nodes{ namespace generators {

/**
 * @brief The F namespace contains branch generator functions. They must all
 * follow the same signature: 
 *      branch_type (const int64_t, void*)
 * 
 * @param order an integer representing the order of the branch in the tree. The
 * root is of order 0.
 * @param kwargs a void pointer to the data structure that contains additional
 * arguments required by the generator. This could be a seed or anything else.
 * @return a branch_type object representing the type of the branch of given 
 * order.
 * 
 * @details the methods implemented in this namespace are:
 * - red: return red branch no matter what (+omega)
 * - green: return green branch no matter what (*)
 * - blue: return blue branch no matter what (-omega)
 * - num: return branch corresponding to the number given in kwargs.
 * -
 */
namespace F {
    branch_type red  (const int64_t order, void* kwargs);
    branch_type green(const int64_t order, void* kwargs);
    branch_type blue (const int64_t order, void* kwargs);
    branch_type num  (const int64_t order, void* kwargs);
}

/**
 * @brief The f namespace contains step generator functions. They must all 
 * follow the same signature:
 *      glm::vec3 (const int64_t, const glm::vec3&, const glm::vec3&)
 * 
 * @param order an integer representing the order of the step in the tree. The 
 * root is of order 0.
 * @param rootpos a vec3 describing the position of the root node.
 * @param kwargs a vec3 describing an additional argument (usually a direction) 
 * used by the generator.
 * @return a vec3 describing the position of a node of given order.
 */
namespace f {
    glm::vec3 linear        (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
    glm::vec3 harmonic      (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
    glm::vec3 quadratic     (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
    glm::vec3 geometric     (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
    glm::vec3 c_quadratic   (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
    glm::vec3 c_geometric   (const int64_t order, const glm::vec3 &rootpos, 
                             const glm::vec3 &kwargs);
}

/**
 * @brief The f_ namespace contains inverse step generator functions. They must 
 * all follow the same signature:
 *      int64_t (const glm::vec3&, const glm::vec3&, 
 *               const glm::vec3&, const glm::vec3&)
 * @warning implementations of functions in the f_ namespace must be the inverse
 *  of the functions with the same name in the f namespace.
 * 
 * @param bottomleft a vec3 describing the bottom left corner of the bounding
 * box.
 * @param topright a vec3 describing the top right corner of the bounding box.
 * @param rootpos a vec3 describing the position of the root node.
 * @param kwargs a vec3 describing an additional argument (usually a direction)
 * used by the generator in the f namespace.
 * @return an integer representing the order of the step in the tree. 
 * @post f::X(f_::X(*args)) is inside the bounding box described by bottomleft
 * and topright for any valid set of arguments *args.
 * 
 * @details the methods implemented in this namespace the same as the ones in 
 * the f namespace.
 */
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

type_gen ALL_RED = F::red;
type_gen ALL_GREEN = F::green;
type_gen ALL_BLUE = F::blue;

struct step_gen {
    glm::vec3   (*a) (const    int64_t, const glm::vec3&, const glm::vec3&);
    int64_t     (*a_)(const glm::vec3&, const glm::vec3&, const glm::vec3&, 
                      const glm::vec3&);
};

step_gen LINEAR;
step_gen HARMONIC;
step_gen QUADRATIC;
step_gen GEOMETRIC { f::harmonic, f_::harmonic };

step_gen CIRCLE_QUADRATIC;
step_gen CIRCLE_GEOMETRIC;
}}}

