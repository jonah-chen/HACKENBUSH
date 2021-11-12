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

// RGBA colors
#define CROSSHAIR_COLOR        1.0f,0.0f,0.0f,1.0f
#define UNSELECTED_NODE_COLOR    1.0f,1.0f,1.0f,0.4f
#define SELECTED_NODE_COLOR    1.0f,1.0f,1.0f,1.0f
#define GROUND_COLOR            0.2f,0.2f,0.2f,1.0f

#include "game/nodes.hpp"
#include "buffer.hpp"
#include <vector>

namespace render::geometry {

class ground : public mesh
{
public:
	explicit ground(shader &shader, float render_distance); // the ground is y=0
private:
	float render_distance_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const override;
};

class crosshair : public mesh
{
public:
	explicit crosshair(shader &shader, float crosshair_size = 0.05f,
					   float aspect = 16.0f / 9.0f);

private:
	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const;
};

class nodes : public mesh
{
public:
	explicit nodes(shader &shader, float width = 0.2f,
				   std::size_t max_nodes = RENDER_LIMIT);

private:
	std::size_t max_nodes_;
	float width_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const override;
};

class edges : public mesh
{
public:
	explicit edges(shader &shader, float line_width = 0.1f,
				   std::size_t max_edges =
				   RENDER_LIMIT);

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

