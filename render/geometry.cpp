#include "geometry.hpp"


static uint32_t *calculate_cube_indices(std::size_t num_cubes)
{
    uint32_t vertex_offset[36] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        0, 4, 5, 0, 5, 1,
        1, 5, 6, 1, 6, 2,
        2, 6, 7, 2, 7, 3,
        3, 7, 4, 3, 4, 0
    };

    uint32_t *indices = new uint32_t[num_cubes * 36];

    for (std::size_t cube = 0; cube < num_cubes; ++cube)
        for (std::size_t elem = 0; elem < 36; ++elem)
            indices[cube*36 + elem] = vertex_offset[elem] + cube * 8;

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
    
    float *vertices = new float[num_nodes * 3];
    for (auto *n : nodes)
    {
        glm::vec3 pos = n->get_pos();

    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW);
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

    unbind();
}

nodes::nodes(float width, std::size_t max_nodes)
    : buffer(GL_TRIANGLES), half_width_(width / 2.0f), max_nodes_(max_nodes)
{
    bind();

    constexpr uint32_t VERTEX_SIZE = 3; 
    // nodes will be drawn as cubes for now.
    // a cube has 6 faces, each with 2 triangles, each with 3 vertices
    // this is 6*2*3 = 36 vertices per node.

    // initialize index buffer
    uint32_t *vertices = calculate_cube_indices(max_nodes);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_nodes * 36 * sizeof(uint32_t), vertices, GL_STATIC_DRAW);
    delete[] vertices;

    unbind();
}


}}