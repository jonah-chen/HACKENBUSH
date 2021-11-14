#include "game.hpp"


hackenbush::~hackenbush()
{
	for (auto *n: node_buf)
		delete n;
	for (auto *e: edge_buf)
		delete e;
}

void hackenbush::load_world(const char *filename, const glm::vec3 &offset)
{
	if (!filename)
	{
		std::cerr << "No filename given...loading default world" << std::endl;
		load_default();
		return;
	}

	worldgen::lut_t lut;
	worldgen::adj_list_t adj_list;

	std::size_t cur_num_nodes = node_buf.size();

	if (!worldgen::parse(filename, lut, adj_list))
		return;

	node_buf.reserve(lut.size());

	for (int32_t node_id = 0; node_id < lut.size(); node_id++)
	{
		game::node *n;

		glm::vec3 &pos = lut[node_id];
		auto &node_type = adj_list[node_id];

		if (node_type.ty == worldgen::node_type::stack_root)
		{
			auto &front_edge = node_type.conn.front();
			n = new game::nodes::stack_root(pos + offset, front_edge.vec_kwargs,
											front_edge.type_gen, front_edge
													.step_gen,
											front_edge.kwargs);
		}
		else
			n = new game::nodes::normal(pos + offset);

		node_buf.push_back(n);

		if (n->get_pos().y == 0.0f)
			grounded_nodes_.insert(n);
	}

	for (int32_t n_p1 = 0; n_p1 < adj_list.size(); ++n_p1)
	{
		auto *p1 = node_buf[n_p1 + cur_num_nodes];
		for (auto &e: adj_list[n_p1].conn)
		{
			auto *p2 = node_buf[e.id + cur_num_nodes];
			game::edge *_e = game::attach(e.type, p1, p2);
			if (_e) edge_buf.insert(_e);
		}
	}
}

void hackenbush::load_default()
{
	glm::vec3 v1(8.0f, 0.0f, 0.0f);
	glm::vec3 v2(8.0f, 1.0f, 0.0f);
	glm::vec3 v3(8.0f, 2.0f, 0.0f);
	glm::vec3 v4(8.0f, 2.0f, 1.0f);

	int32_t *fraction = new int32_t[2];
	fraction[0] = 2;
	fraction[1] = 3;

	auto *n1 = new game::nodes::normal(v1);
	auto *n2 = new game::nodes::normal(v2);
	auto *n3 = new game::nodes::normal(v3);
	auto *n4 = new game::nodes::stack_root(v4, glm::vec3(0.0f, 3.0f, 0.0f),
										   FRACTION,
										   GEOMETRIC, fraction);

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

void hackenbush::command_terminal()
{
	std::cout << "You have discovered the command terminal. Type HELP to see "
				 "the list of commands\n";

	std::string command;

	while (std::cin.good())
	{
		std::cout << ">>";
		std::cin >> command;

		if (command == "KILL")
			exit(0);
		else if (command == "EXIT")
		{
			std::cout << "Exiting... Press ESC again to return to game.\n";
			return;
		}
		else if (command == "HELP")
			std::cout << "Argument List:\n"
						 "EXIT : exit the terminal and go back to the game\n"
						 "LOAD [filename] : Load a world from file\n"
						 "LOGINFO : Print the debug info to the terminal\n"
						 "KILL : exit the game\n";
		else if (command == "LOAD")
		{
			std::string filename;
			std::cin >> filename;
			glm::vec3 offset;
			std::cin >> offset.x >> offset.z;
			offset.y = 0;
			std::cout << "Loading world at " << filename << " with offset ("
					  << offset.x << ",0," << offset.z << ")\n";
			load_world(filename.c_str(), offset);
		}
		else if (command == "LOGINFO")
			std::cout << "Logging info is not implemented\n";
		else
			std::cout << "Invalid command.\n"
						 "Type HELP to see the list of commands\n";
	}
}
