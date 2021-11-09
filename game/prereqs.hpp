/**
 * @file prereqs.hpp
 * @author Jonah Chen
 * @brief Defines the strict prerequisites for the hackenbush game. Includes:
 * - Branches
 * - 
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#define DEFAULT_MAX_BRANCH_DEPTH 1024

#include <cstdint>
#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <glm/glm.hpp>

namespace game
{
/**
 * @brief the branch class describes the any type of branch that exists in this 
 * implementation of hackenbush. Instances of this class are invalid branches, 
 * which describes the ground.
 * 
 * @details branches must implement the following functions:
 * - the call operator
 * - the render method
 * - the log method
 * 
 * The default implementation of the call operator calls the children if they 
 * exist, and an empty set otherwise. This functionality should be implemented 
 * in all derived classes.
 * 
 * The default implementation of the other methods does what is logical, which 
 * is close to nothing.
 */
class branch
{
public:
    using container = std::unordered_set<branch*>;
    /**
     * @brief types of branches in the game
     * - red:   1
     * - green: 0
     * - blue: -1
     */
    enum branch_type : int8_t
    {
        invalid = -127,
        blue    =    1,
        green   =    0,
        red     =   -1
    };

    /**
     * @brief Construct a new branch object with the invalid type and with
     * children located at the a specified position.
     * 
     * @param children pointer to the children branch. Defaults to nullptr which
     * indicates no children.
     * @param root_pos 3d position of the root of the branch. Defaults to the 
     * origin.
     */
    branch(branch *children=nullptr, glm::vec3 root_pos=glm::vec3(0,0,0), 
           uint32_t stability=1) :  type_(invalid), 
                                    children_(children), 
                                    root_pos_(root_pos), 
                                    stability_(stability) {}

    /**
     * Delete the copy constructor and assignment operator because they may
     * cause many undefined and erroneous behaviors.
     * 
     */
    branch(const branch&)               = delete;
    branch &operator=(const branch&)    = delete;

    /**
     * @brief Get the branches that is contained in the volume specified by two 
     * diagonally opposite corners.
     * 
     * @param bottomleft: 3d position of the top left corner of the volume. 
     * Note that the top left corner is the corner with the smallest x, y,
     * and z values.
     * @param topright: 3d position the bottom right corner of the volume. 
     * Note that the bottom right corner is the corner with the largest x, y, 
     * and z values.
     * @param max_depth: 32-bit integer of the maximum number of branches to 
     * return. Defaults to DEFAULT_MAX_BRANCH_DEPTH.
     * 
     * @return an unordered_set of branch references that contains all the 
     * branches contained in the volume which can be used to render branches
     * to the screen and interact with the player.
     * 
     */
    virtual container
    operator() (const glm::vec3 &bottomleft, const glm::vec3 &topright, 
                int32_t max_depth = DEFAULT_MAX_BRANCH_DEPTH);


    /**
     * @brief modify the parameters to the two vertex position of this and
     * only this branch. Note that these are in no particular order.
     * 
     * @param vertex1 3d vector reference to store the first vertex position.
     * @param vertex2 3d vector reference to store the second vertex position.
     * 
     */
    virtual void render (glm::vec3 &vertex1, glm::vec3 &vertex2) const;


    /**
     * @brief write relevant logging info to the output stream.
     * 
     * @param os reference to output stream. Defaults to std::cout.
     */
    virtual void log(std::ostream &os = std::cout) const;

    /**
     * @brief Get the type of the branch.
     * 
     * @return branch_type 
     */
    inline branch_type get_type() const { return type_; }


    /**
     * @brief Update the children of the branch.
     * 
     * @param children pointer to the new children branch.
     * @return pointer to the old children branch that is replaced. 
     */
    branch* update_children(branch *children);

    /**
     * @brief call when the branch is attached to something which is grounded.
     * This incraments the stability of the branch by 1.
     * 
     */
    inline void attach() { ++stability_; }

    /**
     * @brief call when the branch is detached from something which is grounded.
     * This decraments the stability of the branch by 1.
     * 
     * @return true if the branch is still stable, and false if the branch 
     * should fall down.
     */
    inline bool detach() { return --stability_; }


protected:
    branch_type type_;      // type of the branch
    glm::vec3   root_pos_;  // position of the root of the branch
    branch*     children_;  // children of the branch
    uint32_t    stability_; // maximum depth of the branch
};

}