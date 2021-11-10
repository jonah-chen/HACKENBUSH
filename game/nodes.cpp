#include "nodes.hpp"

using namespace game;
using namespace game::nodes;

///////////////////////////////////////////////////////////////////////////////
// Implementation of the normal nodes
///////////////////////////////////////////////////////////////////////////////
void normal::operator()(container &nodes, const glm::vec3 &bottomleft, 
                        const glm::vec3 &topright, int32_t max_depth)
{
    nodes.insert(this);
    if (max_depth)
        for (edge *e : edges)
            if (nodes.find(e->get_other(this))==nodes.end())
                e->get_other(this)->operator()(nodes, bottomleft, topright, max_depth-1);
}

std::unordered_set<edge*> normal::render(int32_t max_breadth) const
{
    return edges;
}

void normal::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
    if (layers>5)
        throw layers;

    os << "normal ";
    os << "@(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
    os << " with " << edges.size() << " edges";
    if (layers)
    {
        os << " to";
        for (edge *e : edges)
        {
            os << std::endl;
            for (uint8_t i=0; i<=counter; ++i)
                os << "\t";
            e->get_other(this)->log(os, layers-1, counter+1);
        }
    }
}

bool normal::attach(edge *e)
{
    edges.insert(e); 
    return true;
}

void normal::detach(edge *e)
{
    edges.erase(e);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the stacked nodes
///////////////////////////////////////////////////////////////////////////////


