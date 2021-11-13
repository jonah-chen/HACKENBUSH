#include "prereqs.hpp"

namespace game {

node::container node::nodes_discard;

glm::vec4 branch_color(branch_type branch)
{
	switch (branch)
	{
		case red:
			return glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		case green:
			return glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		case blue:
			return glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		case invalid:
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		default:
			return glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

edge *attach(branch_type type, node *node1, node *node2)
{
	edge *e = new edge(type, node1, node2);

	bool success1 = node1->attach(e);
	bool success2 = node2->attach(e);

	if (success1 or success2)
		return e;

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