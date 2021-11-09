#include "prereqs.hpp"

using namespace game;

///////////////////////////////////////////////////////////////////////////////
// implement the default functionality for branches
branch::container
branch::operator() (const glm::vec3 &bottomleft, const glm::vec3 &topright, 
                    int32_t max_depth)
{
    return children_ ? children_->operator()(bottomleft, topright) 
                     : container();
}

void branch::render (glm::vec3 &vertex1, glm::vec3 &vertex2) const {}

void branch::log(std::ostream &os) const { 
    os << "Nothing: @";
    os << "(" << root_pos_.x << "," << root_pos_.y << "," << root_pos_.z << ")";
}

branch* branch::update_children(branch *children)
{
    branch *old_children = children_;
    children_ = children;
    return old_children;
}