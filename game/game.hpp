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
#include <list>
#include <cstring>
#include <unordered_map>
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
private:
	class edge_map
	{
	public:
		edge_map() = default;
		edge_map(const edge_map &) = default;
		edge_map(edge_map &&) = default;
		edge_map &operator=(const edge_map &) = default;
		edge_map &operator=(edge_map &&) = default;

		~edge_map()
		{
			for (auto &e : v2p)
				delete e.second;
		}

		inline game::edge *&at(const game::vec6 &loc)
		{ return v2p.at(loc); }

		inline game::vec6 &at(game::edge *e)
		{ return p2v.at(e); }

		edge_map &insert(game::edge *e)
		{
			v2p.insert({e->get_vec6(), e});
			p2v.insert({e, e->get_vec6()});
			return *this;
		}

		void erase(game::edge *e)
		{
			v2p.erase(e->get_vec6());
			p2v.erase(e);
		}

	private:
		std::unordered_map<game::edge *, game::vec6> p2v;
		std::unordered_map<game::vec6, game::edge *> v2p;
	};

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
	edge_map edge_buf;
};