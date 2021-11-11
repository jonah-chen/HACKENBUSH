#include "geometry.hpp"


static std::vector<uint32_t> calculate_cube_indices(std::size_t num_cubes, bool partial=false)
{
    uint32_t vertex_offset[36] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        0, 4, 5, 0, 5, 1,
        1, 5, 6, 1, 6, 2,
        2, 6, 7, 2, 7, 3,
        3, 7, 4, 3, 4, 0
    };

    std::vector<uint32_t> indices;
    indices.reserve(num_cubes * 36);

    for (std::size_t cube = 12*partial; cube < num_cubes; ++cube)
        for (std::size_t elem = 0; elem < 36; ++elem)
            indices.push_back(vertex_offset[elem] + cube * 8);

    return indices;
}


namespace render { namespace geometry {

void ground::enable_vertex_attribs()
{
    glEnableVertexAttribArray(0); // (x,y,z) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0x0);
}

void nodes::enable_vertex_attribs()
{
    glEnableVertexAttribArray(0); // (x,y,z) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0x0);
}

void edges::enable_vertex_attribs()
{
    glEnableVertexAttribArray(0); // (x,y,z) position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0x0);
    glEnableVertexAttribArray(1); // (r,g,b,a) color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(3*sizeof(float)));
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

////////////////////////////////////////////////////////////////////////////////

// NOT DONE!!!!!!
void ground::__update(const game::properties& cur_state)
{
    float positions[] = { // this is not correct
        -render_distance_, 0.0f, -render_distance_,
        -render_distance_, 0.0f, render_distance_,
        render_distance_, 0.0f, render_distance_,
        render_distance_, 0.0f, -render_distance_
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
}


void nodes::__update(const game::properties& cur_state)
{
    // use an node container to store all the nodes
    // and then use the node container to update the buffer
    game::node::container nodes;
    for (auto *e : cur_state.visible_gamestate)
    {
        nodes.insert(e->p1);
        nodes.insert(e->p2);
    }

    std::size_t num_nodes = nodes.size();
    if (num_nodes > max_nodes_)
        throw std::runtime_error(
                                "The number of nodes exceeds the render limit");
    
    std::vector<float> vertices;
    vertices.reserve(num_nodes * 3 * 8);
    for (auto *n : nodes)
    {
        glm::vec3 pos = n->get_pos();
        pos.x -= width_/2.0f;
        pos.y -= width_/2.0f;
        pos.z -= width_/2.0f;

        for (int corner = 0b000; corner <= 0b111; ++corner)
        {
            vertices.push_back(pos.x + (corner & 0b001)*width_);
            vertices.push_back(pos.y + (corner & 0b010)*width_);
            vertices.push_back(pos.z + (corner & 0b100)*width_);
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0x0, vertices.size() * sizeof(float), vertices.data());
}


void edges::__update(const game::properties& cur_state)
{
    // use an edge container to store all the edges
    // and then use the edge container to update the buffer
    game::edge::container &edges = cur_state.visible_gamestate;
    std::size_t num_edges = edges.size();
    if (num_edges > max_edges_)
        throw std::runtime_error(
                                "The number of edges exceeds the render limit");

    std::vector<float> vertices;
    vertices.reserve(num_edges * 7 * 8);

    for (auto *e : edges)
    {
        glm::vec3 p1 = e->p1->get_pos();
        glm::vec3 p2 = e->p2->get_pos();
        glm::vec4 color = branch_color(e->type);

        glm::vec3 dir = p2 - p1;
        // find 2 orthogonal vectors to dir
        glm::vec3 test_vector;
        if (dir.x and dir.y) test_vector.z = 1.0f;
        else                 test_vector.x = 1.0f;

        glm::vec3 ortho1 = glm::normalize(glm::cross(dir, test_vector))*width_;
        glm::vec3 ortho2 = glm::normalize(glm::cross(dir, ortho1))*width_;

        p1 -= ortho1 / 2.0f + ortho2 / 2.0f;
        p2 -= ortho1 / 2.0f + ortho2 / 2.0f;

        for (int corner = 0b00; corner <= 0b11; ++corner)
        {
            vertices.push_back(p1.x + ortho1.x*(corner & 0b01)
                                    + ortho2.x*(corner & 0b10));
            vertices.push_back(p1.y + ortho1.y*(corner & 0b01)
                                    + ortho2.y*(corner & 0b10));
            vertices.push_back(p1.z + ortho1.z*(corner & 0b01)
                                    + ortho2.z*(corner & 0b10));
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
            vertices.push_back(color.a);
        }
        for (int corner = 0b00; corner <= 0b11; ++corner)
        {
            vertices.push_back(p2.x + ortho1.x*(corner & 0b01)
                                    + ortho2.x*(corner & 0b10));
            vertices.push_back(p2.y + ortho1.y*(corner & 0b01)
                                    + ortho2.y*(corner & 0b10));
            vertices.push_back(p2.z + ortho1.z*(corner & 0b01)
                                    + ortho2.z*(corner & 0b10));
            vertices.push_back(color.r);
            vertices.push_back(color.g);
            vertices.push_back(color.b);
            vertices.push_back(color.a);
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0x0, vertices.size() * sizeof(float), vertices.data());
}



////////////////////////////////////////////////////////////////////////////////



ground::ground(float render_distance)
    : buffer(GL_TRIANGLES), render_distance_(render_distance)
{
    // I need to use the position and render distance to figure out what
    // triangle to draw at y = 0.
    
    bind(); // bind the buffers to write to them

    count_ = 6; // 2 triangles per quad, 3 vertices per triangle
    uint32_t indices[6] = {
        0, 1, 2,
        0, 2, 3
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // allocate memory for vertex buffer object
    // 4 vertices per quad * 3 floats per vertex
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    unbind();
}

nodes::nodes(float width, std::size_t max_nodes)
    : buffer(GL_TRIANGLES), width_(width), max_nodes_(max_nodes)
{
    bind();

    // nodes will be drawn as cubes for now.
    // a cube has 6 faces, each with 2 triangles, each with 3 vertices
    // this is 6*2*3 = 36 vertices per node.

    // initialize index buffer
    std::vector<uint32_t> indices = calculate_cube_indices(max_nodes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // allocate memory for the vertex buffer
    // 8 vertices per node * 3 floats per vertex
    glBufferData(GL_ARRAY_BUFFER, max_nodes * 8 * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    unbind();
}


edges::edges(float width, std::size_t max_edges)
    : buffer(GL_LINES), width_(width), max_edges_(max_edges)
{
    bind();

    // initialize index buffer
    std::vector<uint32_t> indices = calculate_cube_indices(max_edges, true);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    // allocate memory for the vertex buffer
    // 8 vertices per node * 7 floats per vertex
    glBufferData(GL_ARRAY_BUFFER, max_edges * 8 * 7 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    unbind();

}

}}