/**
 * @file branches.hpp
 * @author Jonah Chen
 * @brief Defines the different types of branches used in hackenbush. Includes:
 * - Single branch
 * - Multiple, but finitely many branches
 * - Infinite collection of branches like:
 *      - Infinite stacks
 *      -
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "prereqs.hpp"

namespace game { namespace branches {
/**
 * @brief the single class describes a single branch which may have up to 
 * infinitely many children. Note that this class does not have a default 
 * constructor.
 * 
 */
class single : public branch
{
public:
    /**
     * @brief Construct a new single branch.
     * 
     * @param pos1 the position of the first vertex of the branch.
     * @param pos2 the position of the second vertex of the branch.
     * @param type the type of the branch.
     */
    single(const glm::vec3 &root_pos, const glm::vec3 &leaf_pos, 
           branch_type type, branch* children = nullptr);

    single(const single&) = delete;
    single& operator=(const single&) = delete;

    /**
     * @brief Get the branches that is contained in the volume specified
     * by two diagonally opposite corners.
     * 
     * @param bottomleft: 3d position of the top left corner of the volume. 
     * Note that the top left corner is the corner with the smallest x, 
     * y, and z values.
     * @param topright: 3d position the bottom right corner of the 
     * volume. Note that the bottom right corner is the corner with the
     * largest x, y, and z values.
     * @param max_depth: 32-bit integer of the maximum number of 
     * branches to return. Defaults to DEFAULT_MAX_BRANCH_DEPTH.
     * 
     * @return an unordered_set of branch references that contains all 
     * the branches contained in the volume which can be used to render
     * branches to the screen and interact with the player.
     * 
     */
    container
    operator()(const glm::vec3 &bottomleft, 
               const glm::vec3 &topright,
               int32_t max_depth = DEFAULT_MAX_BRANCH_DEPTH) override;

    
    /**
     * @brief modify the parameters to the two vertex position of this
     * and only this branch. Note that these are in no particular order.
     * 
     * @param vertex1 3d vector reference to store pos1_.
     * @param vertex2 3d vector reference to store pos2_.
     * 
     */
    void render(glm::vec3 &vertex1, glm::vec3 &vertex2) const override;

    /**
     * @brief write the debug info "single branch: {pos1_-->pos2_}"" to 
     * the output stream.
     * 
     * @param os reference to output stream. Defaults to std::cout.
     */
    void log(std::ostream &os = std::cout) const override;

private:
    glm::vec3 leaf_pos_;
};

/**
 * @brief the finite class describes finitely many branches which may
 * each have up to infinitely many children. Note that this class does
 * 
 */
class finite : protected branch
{
public:

private:
};

}}