/**
 * @file geometry.hpp
 * @author Jonah Chen
 * @brief contains code to load geometry into vertex and index buffers for
 * rendering.
 * @version 0.1
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#define RENDER_LIMIT 4096

#include "game/nodes.hpp"
#include "buffer.hpp"
#include <vector>

namespace render {
namespace geometry {

class ground : public buffer
{
public:
	ground(float render_distance); // the ground is y=0
private:
	float render_distance_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;
};

class nodes : public buffer
{
public:
	nodes(float width = 0.2f, std::size_t max_nodes = RENDER_LIMIT);

private:
	std::size_t max_nodes_;
	float width_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;
};

class edges : public buffer
{
public:
	edges(float line_width = 0.1f, std::size_t max_edges = RENDER_LIMIT);

private:
	struct vertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	std::size_t max_edges_;
	float width_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;
};

}
}

