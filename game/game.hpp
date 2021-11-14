/**
 * @file game.hpp
 * @author Jonah Chen
 * @brief This file contains the class for the game, which allows the program to
 *  run hackenbush. Also, it should allow future upgrades to allow online 
 * multiplayer play.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "prereqs.hpp"
#include "render/buffer.hpp"
#include <vector>
#include <unordered_map>
#include <list>
#include <cstring>
#include "interaction/input.hpp"
#include "worldgen/parser.hpp"
#include "nodes.hpp"
#include "generators.hpp"

enum player
{
	red_player = 0, blue_player
};

class hackenbush
{
public:
	hackenbush() = default;

	~hackenbush();

	/**
	 * @brief Load a world from file. It is also possible to stack worlds by
	 * loading multiple files concurrently. Commonly, an offset should be
	 * specified to ensure the branches do not overlap.
	 *
	 * @pre The world file must be in the correct format (see parser.hpp)
	 * @pre Branches described in the world file (accounting the specified
	 * offset) should not overlap with branches already loaded into the world.
	 *
	 * @param filename the filename of the world file (.hkb) as a c-string.
	 * @param offset a vec3 describing offset to the coordinates to load the
	 * branches into the world compared to the coordinates specified in the
	 * world file.
	 */
	void load_world(const char *filename,
					const glm::vec3 &offset = glm::vec3());

	/**
	 * @brief Load a default world. Called when a world file is not specified,
	 * or for debugging.
	 *
	 */
	void load_default();

	/**
	 * @brief chop a branch off the world.
	 *
	 * @param edge a pointer to the edge the player chose to chop.
	 * @param player the player who chose to chop the edge.
	 * @return true if the chop is succesful.
	 * @return false if the chop is unsuccessful (i.e. when a player tries to
	 * chop a branch that is not theirs).
	 */
	bool chop(game::edge *edge, player player);

	/**
	 * @brief Get the visible edges to a player with a viewport specified by the
	 *  bottom left and top right corners (of a cube) so the edges can be
	 * rendered or processed in another way.
	 *
	 * @param edges a reference to the container to store the edges.
	 * @param bottomleft a vec3 describing the bottom left corner of the
	 * viewport.
	 * @param topright a vec3 describing the top right corner of the viewport.
	 */
	void
	get_visible_edges(game::edge::container &edges, const glm::vec3 &bottomleft,
					  const glm::vec3 &topright) const;

	/**
	 * @brief Open a command terminal. This is primarily used for debugging (or
	 * server-side modifications in the future).
	 *
	 */
	void command_terminal();

private:
	game::node::container grounded_nodes_;

	std::vector<game::node *> node_buf;
	game::edge::container edge_buf;
	std::unordered_map<glm::vec3, game::node *> grandchild_nodes_;
};