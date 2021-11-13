#include "game.hpp"



hackenbush::~hackenbush()
{
	for (auto *n: node_buf)
		delete n;
	for (auto *e: edge_buf)
		delete e;
}

void hackenbush::load_world(const char *filename)
{
	if (!filename)
	{
		std::cerr << "No filename given...loading default world" << std::endl;
		load_default();
		return;
	}

	worldgen::lut_t lut;
	worldgen::adj_list_t adj_list;

	worldgen::parse(filename, lut, adj_list);

	node_buf.reserve(lut.size());
	for(int32_t node_id = 0 ; node_id < lut.size(); node_id++)
    {
        auto *n = new game::nodes::normal(lut[node_id]);
		node_buf.push_back(n);
		if (n->get_pos().y == 0.0f)
			grounded_nodes_.insert(n);
    }

	for (int32_t n_p1 = 0; n_p1 < adj_list.size(); ++n_p1)
    {
		auto *p1 = node_buf[n_p1];
        for (auto &e : adj_list[n_p1])
        {
            auto *p2 = node_buf[e.id];
            edge_buf.insert(game::attach(e.type, p1, p2));
        }
    }
}

void hackenbush::load_default()
{
	glm::vec3 v1(8.0f, 0.0f, 0.0f);
	glm::vec3 v2(8.0f, 1.0f, 0.0f);
	glm::vec3 v3(8.0f, 2.0f, 0.0f);
	glm::vec3 v4(8.0f, 2.0f, 1.0f);

	auto *n1 = new game::nodes::normal(v1);
	auto *n2 = new game::nodes::normal(v2);
	auto *n3 = new game::nodes::normal(v3);
	auto *n4 = new game::nodes::stack_root(v4, glm::vec3(0.0f, 3.0f, 0.0f),
										   ALL_GREEN,
										   GEOMETRIC, nullptr);

	grounded_nodes_.insert(n1);
	node_buf.push_back(n1);
	node_buf.push_back(n2);
	node_buf.push_back(n3);
	node_buf.push_back(n4);

	edge_buf.insert(game::attach(game::green, n1, n2));
	edge_buf.insert(game::attach(game::blue, n2, n3));
	edge_buf.insert(game::attach(game::red, n2, n4));

	n2->log();

	std::cout << std::endl;
}

void hackenbush::get_visible_edges(game::edge::container &edges,
								   const glm::vec3 &bottomleft,
								   const glm::vec3 &topright) const
{
	edges.clear();
	game::node::container visible_nodes;
	for (game::node *g_node: grounded_nodes_)
		(*g_node)(visible_nodes, bottomleft, topright);
	for (game::node *n: visible_nodes)
		n->render(edges);
}

bool hackenbush::chop(game::edge *edge, player player)
{
	if ((player == blue_player and edge->type != game::red) or
		(player == red_player and edge->type != game::blue))
	{
		game::detach(edge);
		edge_buf.erase(edge);
		return true;
	}
	return false;
}

