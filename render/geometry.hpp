/**
 * @file geometry.hpp
 * @author Jonah Chen
 * @brief contains code to load geometry into vertex and index buffers for
 * rendering.
 * @version 1.0
 * @date 2021-11-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "common/constants.hpp"
#include "game/nodes.hpp"
#include "buffer.hpp"
#include <vector>

namespace render::geometry {

/**
 * @brief The ground is a geometry that is just a quad at y = 0. This will 
 * describe what nodes must be connected to in order to stay alive.
 * 
 * @details This geometry is a quad with vertices at x and z values +/- 
 * render_distance from the player's current x and z position, and y = 0.
 * 
 */
class ground : public mesh
{
public:
	explicit ground(shader &shader, float render_distance);

private:
	float render_distance_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const override;
};


/**
 * @brief The crosshair is just two lines which aid the player in aiming at the 
 * branches.
 * 
 */
class crosshair : public mesh
{
public:
	explicit crosshair(shader &shader, bool player,
					   float crosshair_size = 0.05f,
					   float aspect = 16.0f / 9.0f);

	void switch_player();

private:
	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const override;

	bool is_blue_player;
};

/**
 * @brief The nodes geometry is composed of axis aligned cubes which are 
 * centered at the position of the node. These nodes are rendered to seperate 
 * the edges from each other, which is especially useful when edges of the 
 * same color are connected to each other.
 * 
 */
class nodes : public mesh
{
public:
	explicit nodes(shader &shader, float width = 0.2f,
				   std::size_t max_nodes = RENDER_LIMIT);

protected:
	std::size_t max_nodes_;
	float width_;

	void __update(const game::properties &cur_state) override;

	void enable_vertex_attribs() override;

	void disable_vertex_attribs() override;

	void prepare_shader(shader &shader) const override;
};

/**
 * @brief The selected node have brighter colors to indicate to the player that 
 * they are selected. 
 * 
 * @details It is the same as nodes but with a different color uniform so the 
 * 2 nodes of the selected edge are rendered with a distinctive color. 
 * 
 */
class selected_nodes : public nodes
{
public:
	explicit selected_nodes(shader &shader, float width = 0.2f)
			: nodes(shader, width, 2), type_(game::invalid)
	{}

private:
	void __update(const game::properties &cur_state) override;

	void prepare_shader(shader &shader) const override;

	game::branch_type type_;
};

/**
 * @brief The edges geometry is composed of rectangular prisms which are 
 * connected to each other. Edges are the primary component of the game as they 
 * represent branches which the players can chop.
 * 
 * @warning The edges are usually rendered with a different shader, as the color
 *  is a vertex attribute rather than a uniform.
 * 
 */
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

