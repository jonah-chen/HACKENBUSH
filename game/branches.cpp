#include "branches.hpp"

using namespace game;
using namespace game::branches;

///////////////////////////////////////////////////////////////////////////////
// implement the default functionality for single branches

single::single(const glm::vec3& root_pos, const glm::vec3& leaf_pos,
               branch_type type, branch* children)
    : leaf_pos_(leaf_pos)
{
    root_pos_ = root_pos;
    type_ = type;
    children_ = children;
}

game::branch::container
single::operator() (const glm::vec3 &bottomleft, const glm::vec3 &topright,
                    int32_t max_depth)
{

    if (bottomleft.x < root_pos_.x and bottomleft.y < root_pos_.y and 
        bottomleft.z < root_pos_.z and bottomleft.x < leaf_pos_.x and 
        bottomleft.y < leaf_pos_.y and bottomleft.z < leaf_pos_.z and
        topright.x > root_pos_.x and topright.y > root_pos_.y and 
        topright.z > root_pos_.z and topright.x > leaf_pos_.x and 
        topright.y > leaf_pos_.y and topright.z > leaf_pos_.z) 
    {
        auto result = children_ ? 
                      children_->operator()(bottomleft, topright, max_depth-1) :
                      branch::container();
        result.insert(this);
        return result;
    }

    return branch::operator()(bottomleft, topright, max_depth);
}

void single::render (glm::vec3 &vertex1, glm::vec3 &vertex2) const
{
    vertex1 = root_pos_;
    vertex2 = leaf_pos_;
}

void single::log(std::ostream &os) const
{
    os << "Single branch: ";
    os << "(" << root_pos_.x << "," << root_pos_.y << "," << root_pos_.z << ")";
    os << "-->";
    os << "(" << leaf_pos_.x << "," << leaf_pos_.y << "," << leaf_pos_.z << ")";
}


