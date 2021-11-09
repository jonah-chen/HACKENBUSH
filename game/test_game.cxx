#include "game.hpp"
#include <cassert>
#include <iostream>
#include <string>

#define ENDL std::cout << std::endl

static void test_branch_default() 
{
    game::branch gb; // create object
    glm::vec3 dummy (0.0f, 0.0f, 0.0f);

    // test call and ensure it returns the empty set
    assert(gb(dummy, dummy).size() == 0);

    // test render and ensure dummy isn't modified 
    gb.render(dummy, dummy);
    assert(dummy == glm::vec3(0.0f, 0.0f, 0.0f));

    gb.log();
    ENDL;
}

static void test_branch_single() 
{

    glm::vec3 x1 (0.0f, 0.0f, 0.0f);
    glm::vec3 x2 (0.0f, 1.0f, 0.0f);
    glm::vec3 x3 (0.0f, 1.5f, 0.0f);
    glm::vec3 x4 (0.0f, -1.0f, 0.5f);

    glm::vec3 bottomleft (-1.0f, -1.0f, -1.0f);
    glm::vec3 topright (1.0f, 2.0f, 1.0f);
    glm::vec3 topright2 (1.0f, 1.1f, 1.0f);

    game::branches::single b1_(x1, x2, game::branch::green);
    game::branch &b1 = b1_;

    game::branches::single b2_(x2, x3, game::branch::red, &b1_);
    game::branch &b2 = b2_;

    game::branches::single b3_(x3, x4, game::branch::blue, &b2_);
    game::branch &b3 = b3_;

    // test call
    auto out1 = b1(bottomleft, topright);
    assert(out1.size() == 1);

    auto out2 = b2(bottomleft, topright);
    assert(out2.size() == 2);

    auto out3 = b3(bottomleft, topright);
    assert(out3.size() == 2);

    auto out4 = b2(bottomleft, topright2);
    assert(out4.size() == 1);


    // test render
    glm::vec3 render_buffer[6];
    b1.render(render_buffer[0], render_buffer[1]);
    b2.render(render_buffer[2], render_buffer[3]);
    b3.render(render_buffer[4], render_buffer[5]);

    assert(render_buffer[0] == x1 and render_buffer[1] == x2 or 
           render_buffer[0] == x2 and render_buffer[1] == x1);
    
    assert(render_buffer[2] == x2 and render_buffer[3] == x3 or 
           render_buffer[2] == x3 and render_buffer[3] == x2);
    
    assert(render_buffer[4] == x3 and render_buffer[5] == x4 or
           render_buffer[4] == x4 and render_buffer[5] == x3);

    // test log
    b1.log(); ENDL;
    b2.log(); ENDL;
    b3.log(); ENDL;
}

int main(int argc, char **argv)
{
    test_branch_default();
    test_branch_single();
}