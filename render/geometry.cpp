#include "geometry.hpp"


static std::vector<GLuint> calculate_cube_indices(std::size_t num_cubes,
												  bool partial = false)
{
	GLuint vertex_offset[36] = {
			0, 1, 3, 0, 3, 2,
			4, 5, 7, 4, 7, 6,
			0, 4, 5, 0, 5, 1,
			1, 5, 7, 1, 7, 3,
			3, 7, 6, 3, 6, 2,
			2, 6, 4, 2, 4, 0
	};

	std::vector<GLuint> indices;
	indices.reserve(num_cubes * 36);

	for (std::size_t cube = 0; cube < num_cubes; ++cube)
		for (std::size_t elem = 12 * partial; elem < 36; ++elem)
			indices.push_back(vertex_offset[elem] + cube * 8);

	return indices;
}


namespace render::geometry {

void ground::enable_vertex_attribs()
{
	glEnableVertexAttribArray(0); // (x,y,z) position
}

void nodes::enable_vertex_attribs()
{
	glEnableVertexAttribArray(0); // (x,y,z) position
}

void edges::enable_vertex_attribs()
{
	glEnableVertexAttribArray(0); // (x,y,z) position
	glEnableVertexAttribArray(1); // (r,g,b,a) color
}

void crosshair::enable_vertex_attribs()
{
	glEnableVertexAttribArray(0); // (x,y) SCREEN position
}


void ground::disable_vertex_attribs()
{
	glDisableVertexAttribArray(0);
}

void nodes::disable_vertex_attribs()
{
	glDisableVertexAttribArray(0);
}

void edges::disable_vertex_attribs()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void crosshair::disable_vertex_attribs()
{
	glDisableVertexAttribArray(0);
}


////////////////////////////////////////////////////////////////////////////////

// NOT DONE!!!!!!
void ground::__update(const game::properties &cur_state)
{
	float positions[] = {
			cur_state.pos.x - render_distance_, 0.0f,
			cur_state.pos.z - render_distance_,
			cur_state.pos.x - render_distance_, 0.0f,
			cur_state.pos.z + render_distance_,
			cur_state.pos.x + render_distance_, 0.0f,
			cur_state.pos.z + render_distance_,
			cur_state.pos.x + render_distance_, 0.0f,
			cur_state.pos.z - render_distance_
	};

	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), positions,
				 GL_DYNAMIC_DRAW);
}


void nodes::__update(const game::properties &cur_state)
{
	// use an node container to store all the nodes
	// and then use the node container to update the buffer
	game::node::container nodes;
	for (game::edge *e: cur_state.visible_gamestate)
	{
		nodes.insert(e->p1);
		nodes.insert(e->p2);
	}

	std::size_t num_nodes = nodes.size();
	if (num_nodes > max_nodes_)
		throw std::runtime_error(
				"The number of nodes exceeds the render limit");

	count_ = num_nodes * 6 * 6; // 6 faces, 6 vertices per quad

	std::vector<float> vertices;
	vertices.reserve(num_nodes * 3 * 8);
	for (game::node *n: nodes)
	{
		glm::vec3 pos = n->get_pos();
		pos.x -= width_ / 2.0f;
		pos.y -= width_ / 2.0f;
		pos.z -= width_ / 2.0f;

		for (uint8_t corner = 0b000; corner <= 0b111; ++corner)
		{
			vertices.push_back(pos.x + (bool) (corner & 0b001) * width_);
			vertices.push_back(pos.y + (bool) (corner & 0b010) * width_);
			vertices.push_back(pos.z + (bool) (corner & 0b100) * width_);
		}
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0x0, vertices.size() * sizeof(float),
					vertices.data());
}


void edges::__update(const game::properties &cur_state)
{
	// use an edge container to store all the edges
	// and then use the edge container to update the buffer
	game::edge::container &edges = cur_state.visible_gamestate;
	std::size_t num_edges = edges.size();
	if (num_edges > max_edges_)
		throw std::runtime_error(
				"The number of edges exceeds the render limit");

	count_ = num_edges * 4 * 6; // 4 faces, 6 vertices per quad

	std::vector<vertex> vertices;
	vertices.reserve(count_); // 7 floats per vertex


	for (game::edge *e: edges)
	{
		glm::vec3 p1 = e->p1->get_pos();
		glm::vec3 p2 = e->p2->get_pos();
		glm::vec4 color = branch_color(e->type);

		glm::vec3 dir = p2 - p1;
		// find 2 orthogonal vectors to dir
		glm::vec3 test_vector;
		if (dir.x and dir.y) test_vector.z = 1.0f;
		else test_vector.x = 1.0f;

		glm::vec3 ortho1 = glm::normalize(glm::cross(test_vector, dir));
		glm::vec3 ortho2 = glm::normalize(glm::cross(ortho1, dir)) * width_;
		ortho1 *= width_;

		// get the "bottomleft" vertex position
		p1 -= ortho1 / 2.0f + ortho2 / 2.0f;
		p2 -= ortho1 / 2.0f + ortho2 / 2.0f;

		// bottom vertices
		vertices.push_back({p1, color});
		vertices.push_back({p1 + ortho1, color});
		vertices.push_back({p1 + ortho2, color});
		vertices.push_back({p1 + ortho1 + ortho2, color});

		// top vertices
		vertices.push_back({p2, color});
		vertices.push_back({p2 + ortho1, color});
		vertices.push_back({p2 + ortho2, color});
		vertices.push_back({p2 + ortho1 + ortho2, color});
	}

	glBufferSubData(GL_ARRAY_BUFFER, 0x0, vertices.size() * sizeof(vertex),
					vertices.data());
}



////////////////////////////////////////////////////////////////////////////////

ground::ground(float render_distance)
		: mesh(GL_TRIANGLES), render_distance_(render_distance)
{
	// I need to use the position and render distance to figure out what
	// triangle to draw at y = 0.

	bind(); // bind the buffers to write to them

	count_ = 6; // 2 triangles per quad, 3 vertices per triangle

	GLuint indices[6] = {
			0, 1, 2,
			0, 2, 3
	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
				 GL_STATIC_DRAW);

	// allocate memory for vertex buffer object
	// 4 vertices per quad * 3 floats per vertex
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), nullptr,
				 GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *) 0x0);

	unbind();
}

nodes::nodes(float width, std::size_t max_nodes)
		: mesh(GL_TRIANGLES), width_(width), max_nodes_(max_nodes)
{
	bind();

	// nodes will be drawn as cubes for now.
	// a cube has 6 faces, each with 2 triangles, each with 3 vertices
	// this is 6*2*3 = 36 vertices per node.

	// initialize index buffer
	std::vector<GLuint> indices = calculate_cube_indices(max_nodes);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
				 indices.data(), GL_STATIC_DRAW);

	// allocate memory for the vertex buffer
	// 8 vertices per node * 3 floats per vertex
	glBufferData(GL_ARRAY_BUFFER, max_nodes * 8 * 3 * sizeof(float), nullptr,
				 GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *) 0x0);

	unbind();
}


edges::edges(float width, std::size_t max_edges)
		: mesh(GL_TRIANGLES), width_(width), max_edges_(max_edges)
{
	bind();

	// initialize index buffer
	std::vector<GLuint> indices = calculate_cube_indices(max_edges, true);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
				 indices.data(), GL_STATIC_DRAW);

	// allocate memory for the vertex buffer
	// 8 vertices per node * 7 floats per vertex

	glBufferData(GL_ARRAY_BUFFER, max_edges * 8 * sizeof(vertex), nullptr,
				 GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
						  (void *) offsetof(vertex, pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex),
						  (void *) offsetof(vertex, color));

	unbind();
}

crosshair::crosshair(float crosshair_size)
	: mesh(GL_LINES)
{
	// indices for 2 lines
	float indices[4] = { 0, 1, 2, 3 };

	float vertices[8] = {
			0.5f-crosshair_size, 0.5f,
			0.5f+crosshair_size, 0.5f,
			0.5f, 0.5f-crosshair_size,
			0.5f, 0.5f+crosshair_size
	};

	bind();

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                 GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                          (void *) 0x0);

	unbind();

}

void ground::prepare_shader(shader &shader) const
{
	shader.set_uniform("u_color", 0.2f, 0.2f, 0.2f, 1.0f);
}

void nodes::prepare_shader(shader &shader) const
{
	shader.set_uniform("u_color", 1.0f, 1.0f, 1.0f, 0.4f);
}

void crosshair::prepare_shader(shader &shader) const
{
	// set view projection matrix to the identity matrix
	glm::mat4 identity = glm::mat4(1.0f);
	shader.set_uniform("u_view", identity);
	shader.set_uniform("u_projection", identity);
}

}