/**
 * @file nodes.hpp
 * @author Jonah Chen
 * @brief define the different types of nodes in this implementation of 
 * hackenbush. These include:
 * - Nodes of a `normal` graph.
 * - Nodes of an infinitely deep graph.
 * - Nodes of an infinitely wide graph.
 * @version 0.1
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "prereqs.hpp"

namespace game { namespace nodes {


/**
 * @brief implementation of a node that would belong in a normal, finite graph.
 * this node is designed in a way so that it is compatible with the other more
 * unconventional nodes/branches that are used in this implementation of
 * hackenbush.
 * 
 * @details the node is implemented using an container of all edges that are
 * attached to the node. As game::edges contain both the nodes the edge 
 * connects, this allows the node to be attach to another type (possibly 
 * infinite sequence) of node as well. 
 * 
 */
class normal : public node
{
public:
    /**
     * @brief Construct a new normal node that is not connected to anything.
     * 
     * @param pos the position of the node.
     */
    normal(const glm::vec3 &pos) : node(pos) {}

    normal(const normal&) = delete;
    normal& operator=(const normal&) = delete;


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
    void operator()(container &nodes, const glm::vec3 &bottomleft, 
                    const glm::vec3 &topright, 
                    int32_t max_depth = DEFAULT_MAX_DEPTH) override;


    /**
     * @brief get all the branches attached to this node for use in rendering.
     * 
     * @param max_bredth: arg is irrelevant to a normal node. 
     * Defaults to DEFAULT_MAX_BRANCH_DEPTH.
     * 
     * @return an unordered map of edge pointers that contains all branches.
     */ 
    std::unordered_set<edge*>
    render(int32_t max_depth = DEFAULT_MAX_BREADTH) const override;

    /**
     * @brief write relevant logging info to the output stream.
     * 
     * @pre layers must be less than 6.
     * 
     * @param os reference to output stream. Defaults to std::cout.
     * @param layers: 8-bit integer of the number of recursions to print the 
     * nodes this node is connected to. Defaults to 0.
     */
    virtual void log(std::ostream &os = std::cout, 
                     uint8_t layers=0, uint8_t counter=0) const override;
                     

    /**
     * @brief attach another node to this node via an edge by specifying the 
     * edge connecting them.
     * 
     * @pre the other node must already be created.
     * 
     * @param e pointer to the edge connecting this node to the other node.
     */
    bool attach(edge *e) override;

    /**
     * @brief detach an edge from this node.
     * 
     * @param e pointer to the edge to detach.
     */
    void detach(edge *e) override;

private:
    edge::container edges;
};


/**
 * @brief implementation of a node that would belong in an infinitely tall stack
 * 
 * 
 */
class stack : public node
{
public: 
    using type_gen = 
        branch_type (*)(const int64_t, const branch_type, void*);
    using step_gen = glm::vec3   (*)(const int64_t, const glm::vec3&, void*);

    /**
     * @brief Construct a node that admits a (possibly infinite) stack of 
     * branches that is procedurally generated.
     * 
     * @param pos 3D position of the node.
     * @param tgen type generator function must accept the type and order of 
     * this node and a void pointer of optional arguments. It must return a 
     * branch type for the next branch.
     * @param sgen step generator function must accept the order and current 
     * position of this node and a void pointer of optional arguments. It must 
     * return the position of the next node.
     * @param kwargs optional arguments to be passed to the generator functions.
     * @param order signed integer describing order or index or id of this node.
     * Defaults to 0.
     * @param cap signed integer setting a cap on the depth of this stack. 
     * Defaults to INF(inity), allowing the stack to grow indefinitly
     */
    stack(const glm::vec3 &pos, type_gen tgen, step_gen sgen, 
          void* kwargs = nullptr, int64_t order = 0, int64_t cap=INF)
                :   node(pos), 
                    tgen_(tgen), sgen_(sgen), kwargs_(kwargs), 
                    order_(order), cap_(cap), root_() {}

    stack(const stack&) = delete;
    stack& operator=(const stack&) = delete;

    ~stack();

    stack& operator++();

    void operator()(container &nodes,
                    const glm::vec3 &bottomleft, const glm::vec3 &topright,
                    int32_t max_depth = DEFAULT_MAX_DEPTH) override;


    std::unordered_set<edge*> 
    render(int32_t max_breadth = DEFAULT_MAX_BREADTH) const override; 


    void log(std::ostream &os = std::cout, 
             uint8_t layers=0, uint8_t counter=0) const override;


    bool attach(edge *e) override;


    void detach(edge *e) override;

private:
    type_gen    tgen_;  // Type generator function must accept the type and 
                        // order of this node and a void pointer of optional 
                        // arguments. It must return a branch type for the next 
                        // branch.
    step_gen    sgen_;  // Step generator function must accept the order and 
                        // the current position of this node and a void pointer 
                        // of optional arguments. It must return the position of
                        // the next node.
    int64_t     order_; // The order or index or id of this node.
    int64_t     cap_;   // Cap on the depth of this stack.
    void*       kwargs_;// Optional arguments to be passed to the generator 
                        // functions.
    stack**     root_;  // Pointer to the root of the stack.
};


}}