#pragma once

#include "prereqs.hpp"
#include "render/buffer.hpp"
#include <vector>
#include "interaction/input.hpp"

class hackenbush
{
public:
    hackenbush();
    ~hackenbush();

    void tick();

    void get_visible_edges(game::edge::container& edges, const glm::vec3 &bottomleft, const glm::vec3 &topright) const;

private:
    game::edge::container edges;
	game::node::container grounded_nodes_;
    game::node::container node_buf;
    std::vector<game::edge *> edge_buf;
};
