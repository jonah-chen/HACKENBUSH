/**
 * @file parser.hpp
 * @author your name (you@domain.com)
 * @brief implement the prerequisites to parse world generation files (.hkb) and
 *  generate a world using them.
 * @version 1.0
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <list>
#include "game/prereqs.hpp"
#include "game/nodes.hpp"
#include "game/generators.hpp"

template<>
struct std::hash<glm::vec3>
{
	size_t operator()(const glm::vec3 &vec) const
	{
		return std::hash<float>()(vec.x) ^ std::hash<float>()(vec.y) ^
			   std::hash<float>()(vec.z);
	}
};

namespace worldgen {

enum class node_type
{
	normal = 0,
	stack_root
};

struct edge
{
	int32_t id;
	game::branch_type type;
	game::nodes::generators::type_gen type_gen;
	game::nodes::generators::step_gen step_gen;
	int32_t *kwargs;
	glm::vec3 vec_kwargs;

	edge() = default;

	edge(int32_t id, game::branch_type type) : id(id), type(type)
	{
		type_gen = nullptr;
		step_gen = {nullptr, nullptr};
		kwargs = nullptr;
	}

	edge(int32_t id, game::branch_type type,
		 game::nodes::generators::type_gen type_gen,
		 game::nodes::generators::step_gen step_gen, int32_t *kwargs, const
		 glm::vec3 &vec_kwargs)
			: id(id), type(type), type_gen(type_gen), step_gen(step_gen),
			  kwargs(kwargs), vec_kwargs(vec_kwargs)
	{}
};

struct adj_list_element
{
	node_type ty;
	std::list<edge> conn;

	adj_list_element() : ty(node_type::normal)
	{}
};

using lut_t = std::unordered_map<int32_t, glm::vec3>;
using adj_list_t = std::vector<adj_list_element>;

bool parse(const char *filename, lut_t &node_pos, adj_list_t &adj_list);

}
