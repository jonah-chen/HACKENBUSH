#include "prereqs.hpp"
#include "nodes.hpp"
#include <cassert>
#include <iostream>
#include <string>

#define ENDL std::cout << std::endl

static void test_normal_node()
{
    glm::vec3 v1 (0.0f, 0.0f, 0.0f);
    glm::vec3 v2 (1.0f, 0.0f, 0.0f);
    glm::vec3 v3 (0.0f, 1.0f, 0.0f);
    glm::vec3 v4 (0.0f, 0.0f, 1.0f);
    glm::vec3 v5 (-1.0f, 0.0f, 0.0f);
    glm::vec3 v6 (0.0f, -1.0f, 0.0f);
    glm::vec3 v7 (0.0f, 0.0f, -1.0f);
    glm::vec3 v8 (1.0f, 1.0f, 1.0f);
    glm::vec3 v9 (1.0f, 1.0f, -1.0f);
    glm::vec3 v10 (1.0f, -1.0f, 1.0f);
    glm::vec3 v11 (-1.0f, 1.0f, 1.0f);
    glm::vec3 v12 (-1.0f, -1.0f, 1.0f);

    glm::vec3 bottomleft (-2.0f, -2.0f, -2.0f);
    glm::vec3 topright (2.0f, 2.0f, 2.0f);

    // test creation of some nodes
    game::nodes::normal n1 (v1);
    game::nodes::normal n2 (v2);
    game::nodes::normal n3 (v3);
    game::nodes::normal n4 (v4);
    game::nodes::normal n5 (v5);
    game::nodes::normal n6 (v6);
    game::nodes::normal n7 (v7);
    game::nodes::normal n8 (v8);
    game::nodes::normal n9 (v9);
    game::nodes::normal n10 (v10);
    game::nodes::normal n11 (v11);
    game::nodes::normal n12 (v12);

    game::node &n1_ = n1;
    game::node &n2_ = n2;
    game::node &n3_ = n3;
    game::node &n4_ = n4;
    game::node &n5_ = n5;
    game::node &n6_ = n6;
    game::node &n7_ = n7;
    game::node &n8_ = n8;
    game::node &n9_ = n9;
    game::node &n10_ = n10;
    game::node &n11_ = n11;
    game::node &n12_ = n12;

    // set buffer to collect pointers
    std::vector<game::edge*> buf;
    // test the attach function
    buf.push_back(game::attach(game::red, &n1, &n2));
    buf.push_back(game::attach(game::red, &n1, &n3));
    buf.push_back(game::attach(game::red, &n1, &n4));
    buf.push_back(game::attach(game::red, &n1, &n5));
    buf.push_back(game::attach(game::red, &n1, &n6));
    buf.push_back(game::attach(game::red, &n1, &n7));
    buf.push_back(game::attach(game::red, &n1, &n8));
    buf.push_back(game::attach(game::red, &n1, &n9));
    buf.push_back(game::attach(game::red, &n1, &n10));
    buf.push_back(game::attach(game::red, &n1, &n11));
    buf.push_back(game::attach(game::red, &n1, &n12));
    buf.push_back(game::attach(game::red, &n2, &n3));
    buf.push_back(game::attach(game::red, &n2, &n4));
    buf.push_back(game::attach(game::red, &n2, &n5));
    buf.push_back(game::attach(game::red, &n2, &n6));
    buf.push_back(game::attach(game::red, &n2, &n7));

    n2.log(std::cout , 2);

    for (auto *e : buf)
        game::detach(e);
}
int main(int argc, char **argv)
{
    test_normal_node();

    return 0;
}