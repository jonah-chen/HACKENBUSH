#include "parser.hpp"

#define ERROR {\
std::cerr << "unable to parse line " << line_number << ": " << line\
<< std::endl;\
continue;\
}

static void parse_positions(worldgen::lut_t &node_pos,
							std::unordered_map<glm::vec3, int32_t> &node_ids,
							const char* filename)
{
	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << filename << std::endl;
		return;
	}

	int line_number = 0;
	int32_t node_id = 0;

	std::string line;
	while (std::getline(file, line))
	{
		line_number++; // incrament line number
		if (line.empty() or line[0] == '#') continue; // check for error.

		std::stringstream ss(line);
		std::string command, option;
		game::branch_type branch_type;
		glm::vec3 pos1, pos2;

		ss >> command;
		if (command.size() != 1)
		ERROR

		ss >> option;
		if (option.size() != 1)
		ERROR

		ss >> pos1.x >> pos1.y >> pos1.z;
		if (ss.fail())
		ERROR

		// if the position is not found, update the database to include it.
		if (node_ids.find(pos1) == node_ids.end())
		{
			node_ids[pos1] = node_id;
			node_pos[node_id++] = pos1;
		}

		ss >> command;
		if (command.size() != 2)
		ERROR

		ss >> pos2.x >> pos2.y >> pos2.z;
		if (ss.fail())
		ERROR

		if (node_ids.find(pos2) == node_ids.end() and option[0] != 'f')
		{
			node_ids[pos2] = node_id;
			node_pos[node_id++] = pos2;
		}
	}
	file.close();
}

namespace worldgen {

/**
 * We will try to parse a few standard commands to generate the world.
 * There are different types of branches you can create:
 * 	- 'b' for branch
 * 	- 's' for stack
 * 	- 'f' for flower
 *
 * For option 'b' are different colors too:
 * 	- 'r' for red
 * 	- 'g' for green
 * 	- 'b' for blue
 *
 * For stacks, these options work as well. But to create a stack that represent
 * some number, use 'f'.
 *
 * Afterwards, you must specify the coordinates in 3 dimensions as floats.
 * Make sure to use an arrow (like ->) between the 2 points.
 * For stacks, specify the root first, then specify the vector kwargs. Use the
 * double colon to separate them instead of the arrow.
 *
 * Then, you must specify the type of rendering:
 * 	- 'c' for constant
 * 	- 'h' for harmonic
 * 	- 'q' for quadratic
 * 	- 'g' for geometric
 *
 * The file will be converted into a set of nodes and edges that will be
 * rendered into the world.
 *
 */
bool parse(const char* filename, lut_t &node_pos, adj_list_t &adj_list)
{
	std::unordered_map<glm::vec3, int32_t> node_ids;
	parse_positions(node_pos, node_ids, filename);

	const std::size_t num_nodes = node_ids.size();

	// create adjacency list
	adj_list = adj_list_t(node_ids.size());

	std::ifstream file(filename);
	if (!file.is_open())
	{
		std::cerr << "Could not open file: " << filename << std::endl;
		return false;
	}

	int line_number = 0;
	int32_t node_id = 0;

    std::string line;
    while (std::getline(file, line))
	{
		line_number++; // incrament line number
        if (line.empty() or line[0] == '#') continue; // check for error.

        std::stringstream ss(line);
        std::string command, option;
		game::branch_type branch_type;
		glm::vec3 pos1, pos2;
		int32_t id1, id2;
        ss >> command;
		if (command.size() != 1) ERROR
		ss >> option;
		if (option.size() != 1) ERROR
		switch (option[0])
		{
		case 'r':
			branch_type = game::red;
            break;
		case 'g':
			branch_type = game::green;
			break;
		case 'b':
			branch_type = game::blue;
			break;
		case 'f':
			branch_type = game::invalid;
			break;// need to do more stuff
		default:
			branch_type = game::invalid;
			std::cerr << "Invalid branch type specified at line " <<
			line_number << ": "	<< line << std::endl;
			break;
		}

		ss >> pos1.x >> pos1.y >> pos1.z;
		if (ss.fail()) ERROR

		ss >> command;
		if (command.size() != 2) ERROR

		ss >> pos2.x >> pos2.y >> pos2.z;
		if (ss.fail()) ERROR

		id1 = node_ids.at(pos1);
		if (branch_type == game::invalid)
			adj_list[id1].ty = node_type::stack_root;
        else
		{
			id2 = node_ids.at(pos2);
			adj_list[id1].conn.push_back(edge(id2, branch_type));
		}

		if (command == "::")
		{
			ss >> command; // parse the type of infinite stack
			if (command.size() != 1) ERROR
			switch(command[0])
            {
			case 'c':
				std::cerr << "Constant rendering not implemented yet." << std::endl;
				break;
			case 'h':
				std::cerr << "Harmonic rendering not implemented yet." << std::endl;
				break;
			case 'q':
				std::cerr << "Quadratic rendering not implemented yet." << std::endl;
				break;
			case 'g':
				{
					glm::vec3 leaf = pos1 + pos2;
					auto it = node_ids.find(leaf);

					// if the leaf is not attached to anything else, it won't
					// be in the LUT yet. Thus, add it.
					if (it == node_ids.end())
                    {
						id2 = node_ids.size();
                        node_ids[leaf] = id2;
                        node_pos[id2] = leaf;
                    }
                    else
                        id2 = it->second;
					break;
				}
			default:
				ERROR
			}

			int32_t numerator, denominator;
			ss >> numerator >> denominator;
			if (ss.fail()) ERROR

			edge _edge;

			if (denominator == 0)
			{
				if (numerator > 0)
					_edge = edge(id2, game::red, ALL_RED, GEOMETRIC, nullptr,
								 pos2);
                else if (numerator < 0)
                    _edge = edge(id2, game::blue, ALL_BLUE, GEOMETRIC,
								 nullptr, pos2);
                else
					_edge = edge(id2, game::green, ALL_GREEN, GEOMETRIC,
								 nullptr, pos2);
			}
			else
			{
				int32_t *fraction = new int32_t[2];
				fraction[0] = numerator;
				fraction[1] = denominator;
				_edge = edge(id2, game::blue, FRACTION, GEOMETRIC, fraction,
							 pos2);
			}
			adj_list[id1].conn.push_front(_edge);
		}
    }
	return true;
}

}