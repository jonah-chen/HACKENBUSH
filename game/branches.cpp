#include "prereqs.hpp"

using namespace game;

///////////////////////////////////////////////////////////////////////////////
// implement the default functionality for branches
std::unordered_set<branch*>
branch::operator() (const glm::vec3 &topleft, const glm::vec3 &bottomright)
{
    return children_ ? children_->operator()(topleft, bottomright) 
                     : std::unordered_set<branch*>();
}

void branch::render (glm::vec3 &vertex1, glm::vec3 &vertex2) const {}

void branch::log(std::ostream &os) const { os << "Nothing";}



