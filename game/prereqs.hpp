/**
 * @file prereqs.hpp
 * @author Jonah Chen
 * @brief Defines the strict prerequisites for the hackenbush game. Includes:
 * - Branch types @see branch_type
 * - Edges between nodes (branches) @see edge
 * - Nodes virtual class @see node
 * - 
 * @version 1.0
 * @date 2021-11-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "common/constants.hpp"

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <glm/glm.hpp>

namespace game {

class node; // forward decloration

/**
 * @brief types of branches in the game
 * - red:   1
 * - green: 0
 * - blue: -1
 */
enum branch_type : int8_t
{
	invalid = -127,
	blue = 1,
	green = 0,
	red = -1
};

glm::vec4 branch_color(branch_type type);


/**
 * @brief An edge or branch in the game.
 * 
 * @warning edges are not allowed to be copied, and only one edge should be 
 * created between any two nodes. Creating multiple edges between the same
 * nodes will result in undefined behavior.
 * 
 * @details the container used for this class is std::unordered_set<edge*>.
 */
struct edge
{
	using container = std::unordered_set<edge *>;
	node *p1;
	node *p2;
	branch_type type;

	edge(branch_type type, node *p1, node *p2)
			: p1(p1), p2(p2), type(type)
	{}

	edge(const edge &e) = delete;

	edge &operator=(const edge &e) = delete;

	inline node *get_other(const node *p) const
	{ return p1 == p ? p2 : p1; }
};


/**
 * @brief the abstract class node describes the any type of node that exists in 
 * this implementation of hackenbush. Instances of this class are must
 * 
 * @implements the call operator to recursively collect other nodes that it 
 * directly or indirectly connects to.
 * @implements the render method to collect all the edges it is directly 
 * connected to.
 * @implements the log method to print relevant information about the node.
 * @implements the attach method to attach the node to an edge. Defaults to 
 * always returning false.
 * @implements the detach method to detach an edge, and everything else 
 * connected via that edge. Defaults to doing nothing.
 * 
 * @warning nodes are not allowed to be copied.
 * 
 * @details the container used by this class is std::unordered_set<node*>.
 */
class node
{
public:
	using container = std::unordered_set<node *>;

	/**
	 * @brief Construct a new branch object with the invalid type and with
	 * children located at the a specified position.
	 *
	 * @param pos the 3D position of the node.
	 */
	explicit node(const glm::vec3 &pos) : pos_(pos)
	{}


	/**
	 * Delete the copy constructor and assignment operator because they may
	 * cause many undefined and erroneous behaviors.
	 *
	 */
	node(const node &) = delete;

	node &operator=(const node &) = delete;

	virtual ~node() = default;

	/**
	 * @brief get the nodes that are contained in the volume specified by two
	 * diagonally opposite corners.
	 *
	 * @param nodes: a container where  all the nodes contained in the volume
	 * will be added to by this method. The nodes then can be used to render
	 * branches to the screen and interact with the player.
	 * @param bottomleft: 3d position of the bottom left corner of the volume.
	 * @note the top left corner is the corner with the smallest x, y, and z
	 * values.
	 * @param topright: 3d position the top right corner of the volume.
	 * @note the bottom right corner is the corner with the largest x, y, and z
	 * values.
	 * @param max_depth: 32-bit integer of the maximum number depth to search.
	 * Defaults to DEFAULT_MAX_DEPTH.
	 */
	virtual void operator()(container &nodes, const glm::vec3 &bottomleft,
							const glm::vec3 &topright,
							int32_t max_depth = DEFAULT_MAX_DEPTH) = 0;


	/**
	 * @brief get the vertex positions that will be used for rendering the
	 * branch.
	 * @note render should be really dumb, and should be used after determining
	 * which nodes are needed. The upside of it being dumb is that it should
	 * work universally.
	 *
	 * @param edges: a container where a reference of all the edges attached to
	 * this node will be added to.
	 * @param max_breadth: 32-bit integer of the maximum number of branches to
	 * return. Defaults to DEFAULT_MAX_BRANCH_DEPTH.
	 */
	virtual void render(edge::container &edges,
						int32_t max_breadth = DEFAULT_MAX_BREADTH) = 0;


	/**
	 * @brief write relevant logging info to the output stream.
	 *
	 * @param os reference to output stream. Defaults to std::cout.
	 * @param layers: 8-bit integer of the number of recursions to print the
	 * nodes this node is connected to. Defaults to 0, must be less than 6.
	 * @param counter: 8-bit integer used for intermediate recursion. Defaults
	 * to 0.
	 * @warning counter should NEVER be explicitly passed into the method.
	 */
	virtual void log(std::ostream &os = std::cout,
					 uint8_t layers = 0, uint8_t counter = 0) const = 0;


	/**
	 * @brief attach this node and another node via the specified edge. The
	 * default behavior is to do nothing and return false.
	 *
	 * @warning this method is not meant to be called explicitly.
	 *
	 * @param e pointer to edge used to attach the nodes.
	 * @return true when the edge is attached succesfully.
	 * @return false when the edge cannot be attached.
	 */
	virtual bool attach(edge *e)
	{ return false; };


	/**
	 * @brief detach an edge from this node because either it is chopped by a
	 * player or it is unable to withstand the force of gravity. The default
	 * behavior is to do nothing.
	 *
	 * @warning this method is not meant to be called explicitly.
	 * @warning implementations of this method must NOT deallocate the pointer.
	 *
	 * @param e pointer to the edge to detach.
	 */
	virtual void detach(edge *e)
	{}


	/**
	 * @brief return the position of the node
	 *
	 * @return glm::vec3 of the position of the node.
	 */
	inline glm::vec3 get_pos() const
	{ return pos_; }

protected:
	glm::vec3 pos_; // 3D position of the node.

	static container nodes_discard; // container for nodes that are outside
	// render volume but marked as 'visited'
	// by graph traversal.
};

/**
 * @brief attach the two nodes to each other via an edge, and return a pointer 
 * to the edge.
 * 
 * @param type the type of the edge connecting the two nodes.
 * @param node1 the first node to attach.
 * @param node2 the second node to attach.
 * 
 * @return edge pointer to the edge between the two nodes.
 * 
 * @warning the edge pointer is heap allocated and should be freed elsewhere in 
 * the program.
 */
edge *attach(branch_type type, node *node1, node *node2);


/**
 * @brief detach a edge from each node it is connected to, and deallocate the 
 * memory used by the edge.
 * 
 * @warning the edge pointer should be created with the attach function.
 * 
 * @param e a pointer to the edge to be detached.
 */
void detach(edge *e);


/**
 * @brief detach a edge from each node it is connected to, but do NOT deallocate
 *  the memory used by the edge.
 * 
 * @warning using this method may cause memory leaks.
 * 
 * @param e a pointer to the edge to be detached.
 */
void soft_detach(edge *e);


struct properties
{
	glm::vec3 pos;
	edge::container &visible_gamestate;
	edge *selected_branch;

	properties(glm::vec3 pos, edge::container &visible_gamestate) :
			pos(pos), visible_gamestate(visible_gamestate)
	{}
};

}