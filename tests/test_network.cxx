#include "networking/data.hpp"
#include <map>

int main(int argc, char** argv)
{
    // Create a new network object
    std::map<int,int> map, map2;
    map[1] = 1;
    map[5] = 69;
    map [7] = 3;
    networking::buffer buffer;
    buffer >> map;
    std::cout << "size: " << buffer.size() << std::endl;
    std::cout << buffer << std::endl;
    buffer << map2;
    std::cout << std::dec;
    std::cout << "size: " << map2[5] << std::endl;
    return 0;
}