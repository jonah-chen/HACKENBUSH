/**
 * @file finite.cxx
 * @author Jonah Chen
 * @brief Random world generator based on the mersenne twister. The user can 
 * specify various amount of arguments to generate a world. Execute the program 
 * without any arguments to see usage.
 * @version 0.1
 * @date 2021-11-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <random>
#include <chrono>
#include <fstream>
#include <glm/glm.hpp>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <cstring>

using nodes = std::vector<glm::vec3>;
using edges = std::vector<std::unordered_set<uint32_t>>;

class finite_generator
{
public:
	finite_generator(unsigned int seed,
		   float xz_radius = 10.0f, float y_min = 2.0f, float y_max = 10.0f,
		   float grounded_nodes = 5.0f, float total_nodes = 10.0f,
		   float density = 2.0f, float ground_density_ratio = 1.0f,
		   float blue_ratio = 0.4f, float red_ratio = 0.4f,
		   float node_noise = 2.0f, float density_noise = 2.0f) :
			rng(seed),
			xz_gen(-xz_radius, xz_radius),
			y_gen(y_min, y_max),
			ground_node_gen(grounded_nodes, node_noise),
			node_gen(total_nodes, node_noise),
			density_gen(density, density_noise),
			ground_density_ratio(ground_density_ratio),
			unit(0.0f, 1.0f),
			blue_ratio(blue_ratio),
			red_ratio(red_ratio)
	{
		int num_nodes{}, num_ground_nodes{}, num_edges{};
		while (num_nodes <= 0)
			num_nodes = node_gen(rng);
		while (num_ground_nodes <= 0)
			num_ground_nodes = ground_node_gen(rng);
		while (num_edges <= 0)
			num_edges = density_gen(rng) * num_nodes;

		const float real_ground_ratio = (float)num_ground_nodes / (float)
				num_nodes;
		const int total_ground_edges = std::max((int)(num_edges *
				real_ground_ratio), num_ground_nodes);
		const int extra_ground_edges = total_ground_edges - num_ground_nodes;
		const int air_edges = num_edges - total_ground_edges;


		// generate grounded nodes
		for (int i{}; i < num_ground_nodes; ++i)
			_nodes.push_back(glm::vec3(xz_gen(rng), 0, xz_gen(rng)));
		// generate other nodes
		for (int i{}; i < num_nodes - num_ground_nodes; ++i)
			_nodes.push_back(glm::vec3(xz_gen(rng), y_gen(rng), xz_gen(rng)));

		// generate edges
        _edges = edges(num_nodes);
		// generate an edge from every grounded node to another non-grounded
		// node
		for (int grounded_i{}; grounded_i < num_ground_nodes; ++grounded_i)
		{
			const unsigned int other = num_ground_nodes + rng() %
                    (num_nodes - num_ground_nodes);
			_edges[grounded_i].insert(other);
		}

		// generate the extra edges from grounded nodes
		for (int i{}; i < extra_ground_edges; ++i)
        {
            const unsigned int grounded_i = rng() % num_ground_nodes;
            const unsigned int other = num_ground_nodes + rng() %
                    (num_nodes - num_ground_nodes);
            _edges[grounded_i].insert(other);
        }

		// generate edges from air nodes to air nodes
		for (int i{}; i < air_edges; ++i)
        {
            unsigned int node1{}, node2{};
            while (node1 == node2)
            {
                node1 = num_ground_nodes + rng() % (num_nodes - num_ground_nodes);
                node2 = num_ground_nodes + rng() % (num_nodes - num_ground_nodes);
            }
            
            _edges[std::min(node1, node2)].insert(std::max(node1, node2));
        }
	}

    /**
     * @brief write the generated world to a file.
     * 
     * @pre the file must be opened in write mode.
     * 
     * @param out_file the file which is open to write to. 
     */
	void operator<< (std::ofstream &out_file)
	{
		glm::vec3 pos1, pos2;
		for (uint32_t node_i{}; node_i < _edges.size(); ++node_i)
		{
			pos1 = _nodes[node_i];
			for (uint32_t node2 : _edges[node_i])
            {
                pos2 = _nodes[node2];
                out_file << "b " << gen_color() << " " << pos1.x << " " << pos1.y
                        << " " << pos1.z << " -> " << pos2.x << " " << pos2.y <<
                        " " << pos2.z << std::endl;
            }
		}
	}

private:
	nodes _nodes;
	edges _edges;
	std::mt19937_64 rng;
	std::uniform_real_distribution<float> xz_gen;
	std::uniform_real_distribution<float> y_gen;
	std::normal_distribution<float> ground_node_gen;
	std::normal_distribution<float> node_gen;
	std::normal_distribution<float> density_gen;
	std::uniform_real_distribution<float> unit;
	float ground_density_ratio;
	float blue_ratio, red_ratio;

	char gen_color()
	{
		const float num = unit(rng);
		if (num < blue_ratio)
            return 'b';
        else if (num < blue_ratio + red_ratio)
            return 'r';
        else
            return 'g';
	}
};

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::cout << "Usage: ./finite.out [filename]\n"
        "Optional Arguments: [--seed] [--xzradius] [--ymin] [--ymax]\n"
        "                    [--grounded-nodes] [--nodes]\n"
        "                    [--density] [--ground-density]\n"
        "                    [--blue-ratio] [--red_ratio]\n"
        "                    [--node-noise] [--density_noise]\n" << std::endl;
        exit(0);
    }
    if (argc == 2)
    {
        std::ofstream out_file(argv[1]);
        if (out_file.is_open())
        {
            finite_generator(std::chrono::system_clock::now().time_since_epoch().count()) << out_file;
            exit(0);
        }
    }

    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    float xz_radius = 10.0f;
    float y_min = 2.0f;
    float y_max = 10.0f;
    float grounded_nodes = 5.0f;
    float total_nodes = 20.0f;
    float density = 2.0f;
    float ground_density_ratio = 1.0f;
    float blue_ratio = 0.4f;
    float red_ratio = 0.4f;
    float node_noise = 2.0f;
    float density_noise = 2.0f;
    char *filename;

    if (argc % 2 == 1)
    {
        std::cout << "Error: Invalid number of arguments" << std::endl;
        exit(1);
    }

    int arg = 1;
    while (arg < argc)
    {
        if (strcmp("--seed",argv[arg]) == 0)
        {
            seed = atoi(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--xzradius",argv[arg]) == 0)
        {
            xz_radius = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--ymin",argv[arg]) == 0)
        {
            y_min = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--ymax",argv[arg]) == 0)
        {
            y_max = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--grounded-nodes",argv[arg]) == 0)
        {
            grounded_nodes = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--nodes",argv[arg]) == 0)
        {
            total_nodes = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--density",argv[arg]) == 0)
        {
            density = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--ground-density",argv[arg]) == 0)
        {
            ground_density_ratio = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--blue-ratio",argv[arg]) == 0)
        {
            blue_ratio = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--red-ratio",argv[arg]) == 0)
        {
            red_ratio = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--node-noise",argv[arg]) == 0)
        {
            node_noise = atof(argv[arg+1]);
            arg+=2;
        }
        else if (strcmp("--density-noise",argv[arg]) == 0)
        {
            density_noise = atof(argv[arg+1]);
            arg+=2;
        }
        else
            filename = argv[arg++];
    }

    std::ofstream out_file(argv[1]);
    if (out_file.is_open())
    {
        std::cout << "Seed: " << seed << std::endl;
        std::cout << "XZ Radius: " << xz_radius << std::endl;
        std::cout << "Y Min: " << y_min << std::endl;
        std::cout << "Y Max: " << y_max << std::endl;
        std::cout << "Grounded Nodes: " << grounded_nodes << std::endl;
        std::cout << "Total Nodes: " << total_nodes << std::endl;
        std::cout << "Density: " << density << std::endl;
        std::cout << "Ground Density Ratio: " << ground_density_ratio << std::endl;
        std::cout << "Blue Ratio: " << blue_ratio << std::endl;
        std::cout << "Red Ratio: " << red_ratio << std::endl;
        std::cout << "Node Noise: " << node_noise << std::endl;
        std::cout << "Density Noise: " << density_noise << std::endl;
        std::cout << "Filename: " << filename << std::endl;
        
        finite_generator(seed, xz_radius, y_min, y_max, grounded_nodes, total_nodes, density,
                ground_density_ratio, blue_ratio, red_ratio, node_noise, density_noise) << out_file;
        exit(0);
    }
    else
    {
        std::cout << "Error: Could not open file" << std::endl;
        exit(1);
    }
}

