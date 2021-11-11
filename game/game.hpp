#pragma once
#include "prereqs.hpp"
#include "interaction/input.hpp"

class hackenbush 
{
public:
    hackenbush();

private:
    game::properties cur_state_;
    GLFWwindow* window_;
};