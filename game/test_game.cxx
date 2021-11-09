#include "game.hpp"
#include <cassert>
#include <iostream>
#include <string>

static void test_branch_default() 
{
    game::branch gb; // create object
    glm::vec3 dummy (0.0f, 0.0f, 0.0f);

    // test call and ensure it returns the empty set
    assert(gb(dummy, dummy).size() == 0);

    // test render and ensure dummy isn't modified 
    gb.render(dummy, dummy);
    assert(dummy == glm::vec3(0.0f, 0.0f, 0.0f));
}

int main(int argc, char **argv)
{
    test_branch_default();
}