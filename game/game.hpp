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

	void load_world(const char *filename);
	void load_default();

	void tick();

	bool chop(game::edge *edge, player player);

	void
	get_visible_edges(game::edge::container &edges, const glm::vec3 &bottomleft,
					  const glm::vec3 &topright) const;

private:
	game::node::container grounded_nodes_;

	std::vector<game::node *> node_buf;
	game::edge::container edge_buf;
};
