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
     * @brief get the vertex positions that will be used for rendering the 
     * branch.
     * 
     * @param root: reference to store the position of the root of the branch.
     * @param max_bredth: arg is irrelevant to a normal node. 
     * Defaults to DEFAULT_MAX_BRANCH_DEPTH.
     * 
     * @return a vector of glm::vec3 that contains the leaf vertex positions.
     */ 
    std::vector<glm::vec3>
    render(glm::vec3 &root, 
           int32_t max_depth = DEFAULT_MAX_BREADTH) const override;

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



}}