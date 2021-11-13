#pragma once

#include "prereqs.hpp"
#include "render/buffer.hpp"
#include <vector>
#include "interaction/input.hpp"

enum player
{
	red_player = 0, blue_player
};

class hackenbush
{
public:
	hackenbush();

	~hackenbush();

	void tick();

	void chop(game::edge *edge, player player);

	void
	get_visible_edges(game::edge::container &edges, const glm::vec3 &bottomleft,
					  const glm::vec3 &topright) const;

private:
	game::node::container grounded_nodes_;

	std::vector<game::node *> node_buf;
	std::vector<game::edge *> edge_buf;
};
