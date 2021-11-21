#include "prereqs.hpp"

namespace game {

bool vec6::operator==(const vec6& other) const 
{ return v1 == other.v1 and v2 == other.v2; }

vec6 edge::get_vec6() const
{ return vec6(p1->get_pos(), p2->get_pos()); }

node::container node::nodes_discard;

glm::vec4 branch_color(branch_type branch)
{
	switch (branch)
	{
	case red: return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	case green: return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	case blue: return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	case invalid: return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	default: return glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	}
}

edge *attach(branch_type type, node *node1, node *node2)
{
	edge *e = new edge(type, node1, node2);

	bool success1 = node1->attach(e);
	bool success2 = node2->attach(e);

	if (success1)
		return e;

	if (success2)
		node2->detach(e);

	delete e;
	return nullptr;
}

void detach(edge *e)
{
	soft_detach(e);
	delete e;
}

void soft_detach(edge *e)
{
	e->p1->detach(e);
	e->p2->detach(e);
}

}