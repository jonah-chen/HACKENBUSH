#include "game.hpp"
#include "game/nodes.hpp"
#include "game/generators.hpp"

hackenbush::hackenbush()
{
    glm::vec3 v1(8.0f, 0.0f, 0.0f);
	glm::vec3 v2(8.0f, 1.0f, 0.0f);
	glm::vec3 v3(8.0f, 2.0f, 0.0f);
	glm::vec3 v4(8.0f, 2.0f, 1.0f);

    auto *n1 = new game::nodes::normal(v1);
    auto *n2 = new game::nodes::normal(v2);
    auto *n3 = new game::nodes::normal(v3);
    auto *n4 = new game::nodes::stack_root (v4, glm::vec3(0.0f, 3.0f, 0.0f), ALL_GREEN,
							   GEOMETRIC, nullptr);
    grounded_nodes_.insert(n1);
    node_buf.insert(n1);
    node_buf.insert(n2);
	node_buf.insert(n3);
	node_buf.insert(n4);

	edge_buf.push_back(game::attach(game::green, n1, n2));
	edge_buf.push_back(game::attach(game::blue, n2, n3));
	edge_buf.push_back(game::attach(game::red, n2, n4));
    
	n2->log();

	std::cout << std::endl;
}

hackenbush::~hackenbush()
{
}

void hackenbush::get_visible_edges(game::edge::container& edges, const glm::vec3 &bottomleft, const glm::vec3 &topright) const
{
    game::node::container visible_nodes;
    for (game::node *g_node : grounded_nodes_)
        (*g_node)(visible_nodes, bottomleft, topright);
	for (game::node *n: visible_nodes)
		n->render(edges);
}