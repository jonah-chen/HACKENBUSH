#include "prereqs.hpp"

using namespace game;

edge* node::attach(node *other, branch_type type)
{
    edge *e = new edge(type, this, other);
    attach(e);
    return e;
}