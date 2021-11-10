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

#define DEFAULT_MAX_DEPTH 1024
#define DEFAULT_MAX_BREADTH 1024

#include <cstdint>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <glm/glm.hpp>

namespace game
{

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
 * @brief An edge or branch in the game.
 * 
 * @details the unique copy of an edge between two nodes. The copy 
 * constructor and assignment operator are deleted because they will break
 * many search functionalty since the pointers to the edge will be used.
 */
struct edge 
{
    using container = std::vector<edge*>;
    node *p1;
    node *p2;
    branch_type type;
    edge(branch_type type, node *p1, node *p2) 
        : p1(p1), p2(p2), type(type) {}
    
    edge(const edge &e) = delete;
    edge &operator=(const edge &e) = delete;

    inline node *get_other(const node *p) const{ return p1 == p ? p1 : p2; }
};


/**
 * @brief the node class describes the any type of node that exists in this 
 * implementation of hackenbush. Instances of this class are.
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
class node
{
public:
    using container = std::unordered_set<node*>;

    /**
     * @brief Construct a new branch object with the invalid type and with
     * children located at the a specified position.
     * 
     * @param pos the 3D position of the node.
     */
    node(const glm::vec3 &pos) : pos_(pos) {}


    /**
     * Delete the copy constructor and assignment operator because they may
     * cause many undefined and erroneous behaviors.
     * 
     */
    node(const node&)               = delete;
    node &operator=(const node&)    = delete;


    /**
     * @brief get the nodes that are contained in the volume specified by two 
     * diagonally opposite corners.
     * 
     * @param bottomleft: 3d position of the bottom left corner of the volume. 
     * @note the top left corner is the corner with the smallest x, y, and z 
     * values.
     * @param topright: 3d position the top right corner of the volume. 
     * @note the bottom right corner is the corner with the largest x, y, and z 
     * values.
     * @param max_depth: 32-bit integer of the maximum number depth to search. 
     * Defaults to DEFAULT_MAX_DEPTH.
     * 
     * @return a container of references to all the nodes contained in the 
     * volume which can be used to render branches to the screen and interact 
     * with the player.
     */
    virtual void operator()(container &nodes, const glm::vec3 &bottomleft, 
                            const glm::vec3 &topright, 
                            int32_t max_depth = DEFAULT_MAX_DEPTH)=0;


    /**
     * @brief get the vertex positions that will be used for rendering the 
     * branch.
     * 
     * @param root: reference to store the position of the root of the branch.
     * @param max_bredth: 32-bit integer of the maximum number of branches to
     * return. Defaults to DEFAULT_MAX_BRANCH_DEPTH.
     * 
     * @return a vector of glm::vec3 that contains the leaf vertex positions.
     */
    virtual std::vector<glm::vec3> 
    render(glm::vec3 &root, int32_t max_breadth = DEFAULT_MAX_BREADTH) const=0;


    /**
     * @brief write relevant logging info to the output stream.
     * 
     * @param os reference to output stream. Defaults to std::cout.
     * @param layers: 8-bit integer of the number of recursions to print the 
     * nodes this node is connected to. Defaults to 0, must be less than 6.
     */
    virtual void log(std::ostream &os = std::cout, 
                     uint8_t layers = 0)            const=0;
    
    /**
     * @brief return the position of the node
     * 
     * @return glm::vec3 of the position of the node.
     */
    inline glm::vec3 get_pos() const { return pos_; }


protected:
    glm::vec3 pos_; // 3D position of the node.
};

}