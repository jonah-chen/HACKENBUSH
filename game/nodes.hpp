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

// define the rate geometric series shrinks
#define GEOMETRIC_CONSTANT 0.9f

#include <map>
#include <cmath>
#include <limits>
#include "prereqs.hpp"

// hash struct for pair of int32_t
template<>
struct std::hash<std::pair<int32_t, int32_t>>
{
	size_t operator()(const std::pair<int32_t, int32_t> &p) const
	{
		return hash<int32_t>()(p.first) ^ hash<int32_t>()(p.second);
	}
};

namespace game::nodes {

class normal;

class stack;

class stack_root;

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
	explicit normal(const glm::vec3 &pos, node *parent = nullptr) : node(pos)
	{}

	normal(const normal &) = delete;

	normal &operator=(const normal &) = delete;


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
	 * @param edges: a container where a reference of all the edges attached to
	 * this node will be added to.
	 * @param max_breadth: arg is irrelevant to normal node. Defaults to
	 * DEFAULT_MAX_BRANCH_DEPTH.
	 *
	 */
	void render(edge::container &edges,
				int32_t max_breadth = DEFAULT_MAX_BREADTH) override;

	/**
	 * @brief write relevant logging info to the output stream.
	 *
	 * @pre layers must be less than 6.
	 *
	 * @param os reference to output stream. Defaults to std::cout.
	 * @param layers: 8-bit integer of the number of recursions to print the
	 * nodes this node is connected to. Defaults to 0.
	 * @param counter: 8-bit integer used for intermediate recursion. Defaults
	 * to 0.
	 * @warning counter should NEVER be explicitly passed into the method.
	 */
	virtual void log(std::ostream &os = std::cout,
					 uint8_t layers = 0, uint8_t counter = 0) const override;


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
	edge::container edges_;
};

///////////////////////////////////////////////////////////////////////////////
namespace generators {

/**
 * @brief The F namespace contains branch generator functions. They must all
 * follow the same signature: 
 *      branch_type (const int64_t, void*)
 * 
 * @param order an integer representing the order of the branch in the tree. The
 * root is of order 0.
 * @param kwargs a void pointer to the data structure that contains additional
 * arguments required by the generator. This could be a seed or anything else.
 * @return a branch_type object representing the type of the branch between a 
 * node of order `order` and a node of order `order + 1`.
 * 
 * @details the methods implemented in this namespace are:
 * - red: return red branch no matter what (+omega)
 * - green: return green branch no matter what (*)
 * - blue: return blue branch no matter what (-omega)
 * - num: return branch corresponding to the number given in kwargs.
 * -
 */
namespace F {
branch_type red(const int64_t order, void *kwargs);

branch_type green(const int64_t order, void *kwargs);

branch_type blue(const int64_t order, void *kwargs);

branch_type fraction(const int64_t order, void *kwargs);
}

/**
 * @brief The f namespace contains step generator functions. They must all 
 * follow the same signature:
 *      glm::vec3 (const int64_t, const glm::vec3&, const glm::vec3&)
 * 
 * @param order an integer representing the order of the step in the tree. The 
 * root is of order 0. The limit can be found by using order=INF.
 * @param rootpos a vec3 describing the position of the root node.
 * @param kwargs a vec3 describing an additional argument (usually a direction) 
 * used by the generator.
 * @return a vec3 describing the position of a node of given order.
 * @return a vec3 with x, y, and z values equal to QUIET NAN if the generator
 * diverges.
 */
namespace f {
glm::vec3 linear(const int64_t order, const glm::vec3 &rootpos,
				 const glm::vec3 &kwargs);

glm::vec3 harmonic(const int64_t order, const glm::vec3 &rootpos,
				   const glm::vec3 &kwargs);

glm::vec3 quadratic(const int64_t order, const glm::vec3 &rootpos,
					const glm::vec3 &kwargs);

glm::vec3 geometric(const int64_t order, const glm::vec3 &rootpos,
					const glm::vec3 &kwargs);

glm::vec3 c_quadratic(const int64_t order, const glm::vec3 &rootpos,
					  const glm::vec3 &kwargs);

glm::vec3 c_geometric(const int64_t order, const glm::vec3 &rootpos,
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
namespace f_ {
int64_t linear(const glm::vec3 &bottomleft, const glm::vec3 &topright,
			   const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t harmonic(const glm::vec3 &bottomleft, const glm::vec3 &topright,
				 const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t quadratic(const glm::vec3 &bottomleft, const glm::vec3 &topright,
				  const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t harmonic(const glm::vec3 &bottomleft, const glm::vec3 &topright,
				 const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t geometric(const glm::vec3 &bottomleft, const glm::vec3 &topright,
				  const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t c_quadratic(const glm::vec3 &bottomleft, const glm::vec3 &topright,
					const glm::vec3 &rootpos, const glm::vec3 &kwargs);

int64_t c_geometric(const glm::vec3 &bottomleft, const glm::vec3 &topright,
					const glm::vec3 &rootpos, const glm::vec3 &kwargs);
}

using type_gen = branch_type (*)(const int64_t, void *);

struct step_gen
{
	glm::vec3 (*a)(const int64_t order, const glm::vec3 &rootpos,
				   const glm::vec3 &kwargs);

	int64_t (*a_)(const glm::vec3 &bottomleft, const glm::vec3 &topright,
				  const glm::vec3 &rootpos, const glm::vec3 &kwargs);
};

}


/**
 * @brief implementation of a node that would belong in an infinitely tall stack
 *  that is NOT attached to any other node outside the stack.
 * 
 * @warning this class is not meant to be used directly.
 * 
 */
class stack : public node
{
public:
	using container = std::map<int32_t, stack *>; // mapping order to children

	stack(const glm::vec3 &pos, stack_root *root, int64_t order) :
			node(pos), root_(root), order_(order)
	{};

	stack(const stack &) = delete;

	stack &operator=(const stack &) = delete;

	/**
	 * @brief THIS METHOD SHOULD NOT BE CALLED!
	 * @throw not implemented error.
	 */
	void operator()(node::container &nodes,
					const glm::vec3 &bottomleft, const glm::vec3 &topright,
					int32_t max_depth = DEFAULT_MAX_DEPTH) override;


	void render(edge::container &edges,
				int32_t max_breadth = DEFAULT_MAX_BREADTH) override;


	void log(std::ostream &os = std::cout,
			 uint8_t layers = 0, uint8_t counter = 0) const override;


	bool attach(edge *e) override;

	void detach(edge *e) override;

protected:
	int64_t order_; // The order or index or id of this node.
	stack_root *root_;  // Pointer to the root of the stack.
};


class stack_root : public stack
{
public:
	// Lookup table for the binary representation of repeating fractions.
	static std::unordered_map<std::pair<int32_t, int32_t>, std::vector<bool>>
			fraction_lut;

	/**
	 * @brief Construct a node that admits a (possibly infinite) stack of
	 * branches that is procedurally generated.
	 *
	 * @param pos 3D position of the node.
	 * @param tgen type generator function following the signature defined in
	 * generators.hpp.
	 * @param sgen step generator function following the signature defined in
	 * generators.hpp.
	 * @param kwargs optional arguments to be passed to the generator functions.
	 * @param order signed integer describing order or index or id of this node.
	 * Defaults to 0.
	 * @param cap signed integer setting a cap on the depth of this stack.
	 * Defaults to INF(inity), allowing the stack to grow indefinitly
	 */
	stack_root(const glm::vec3 &pos, const glm::vec3 &vec_kwargs,
			   generators::type_gen tgen, generators::step_gen sgen,
			   void *kwargs, int64_t order = 0, int64_t cap = INF);

	stack_root(const stack_root &) = delete;

	stack_root &operator=(const stack_root &) = delete;

	~stack_root() override;

	stack *operator[](std::size_t i);

	edge *__render(int32_t order = 0, stack *ptr = nullptr, bool next = true);

	node *get_grandchild() const;

	void operator()(node::container &nodes,
					const glm::vec3 &bottomleft, const glm::vec3 &topright,
					int32_t max_depth = DEFAULT_MAX_DEPTH) override;

	void render(edge::container &edges,
				int32_t max_breadth = DEFAULT_MAX_BREADTH) override;

	void log(std::ostream &os = std::cout,
			 uint8_t layers = 0, uint8_t counter = 0) const override;

	bool attach(edge *e) override;

	void detach(edge *e) override;

	void detach(int64_t order);

private:
	container children_;
	node *grandchild_; // this is just a normal node.
	generators::type_gen tgen_;
	generators::step_gen sgen_;
	int64_t cap_;
	glm::vec3 vec_kwargs_;
	void *kwargs_;// Optional arguments to be passed to the generator
	// functions.
};


}