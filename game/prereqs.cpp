#include "prereqs.hpp"

namespace game {

edge* game::attach(branch_type type, node *node1, node *node2)
{
    edge *e = new edge(type, node1, node2);
    

    bool success1 = node1->attach(e);
    bool success2 = node2->attach(e);
    
    if (success1 or success2)
        return e;
    
    delete e;
    return nullptr;
}

void game::detach(edge *e)
{
    soft_detach(e);
    delete e;
}

void game::soft_detach(edge *e)
{
    e->p1->detach(e);
    e->p2->detach(e);
}

}