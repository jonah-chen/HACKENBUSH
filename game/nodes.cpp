#include "nodes.hpp"

using namespace game;
using namespace game::nodes;

#define IN(X,BOT_L,TOP_R) ((X).x >= (BOT_L).x && (X).x <= (TOP_R).x && \
                           (X).y >= (BOT_L).y && (X).y <= (TOP_R).y && \
                           (X).z >= (BOT_L).z && (X).z <= (TOP_R).z)

///////////////////////////////////////////////////////////////////////////////
// Implementation of the normal nodes
///////////////////////////////////////////////////////////////////////////////
void normal::operator()(container &nodes, const glm::vec3 &bottomleft, 
                        const glm::vec3 &topright, int32_t max_depth)
{
    if (IN(pos_, bottomleft, topright))
        nodes.insert(this);
    if (max_depth)
        for (edge *e : edges)
            if (nodes.find(e->get_other(this))==nodes.end())
                e->get_other(this)->operator()(nodes, bottomleft, topright, max_depth-1);
}

edge::container normal::render(int32_t max_breadth)
{
    return edges;
}

void normal::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
    if (layers>5)
        throw layers;

    os << "normal ";
    os << "@(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
    os << " with " << edges.size() << " edges";
    if (layers)
    {
        os << " to";
        for (edge *e : edges)
        {
            os << std::endl;
            for (uint8_t i=0; i<=counter; ++i)
                os << "\t";
            e->get_other(this)->log(os, layers-1, counter+1);
        }
    }
}

bool normal::attach(edge *e)
{
    edges.insert(e); 
    return true;
}

void normal::detach(edge *e)
{
    edges.erase(e);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the stacked nodes
///////////////////////////////////////////////////////////////////////////////

void stack::operator()(node::container &nodes, const glm::vec3 &bottomleft, 
                       const glm::vec3 &topright, int32_t max_depth)
{
    throw "not implemented because stack objects should not be called!";
}

// return the edge that is BOTH moving away or towards the root
edge::container stack::render(int32_t max_breadth)
{
    edge::container edges;
    edge* prev = root_->__render(order_, this, false);
    edge* next = root_->__render(order_, this, true);
    if (prev) edges.insert(prev);
    if (next) edges.insert(next);
    return edges;
}

void stack::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
    os << "stack #" << order_;
    os << " @(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
}

bool stack::attach(edge *e)
{
    std::cout << "cannot attach edge to the middle of an infinite stack"
                << std::endl;
    throw "this is not implemented. please design your game differently";
}

void stack::detach(edge *e)
{
    root_->detach(order_);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the stack root
///////////////////////////////////////////////////////////////////////////////
stack_root::stack_root(const glm::vec3 &pos, const glm::vec3 &vec_kwargs,
                       generators::type_gen tgen, generators::step_gen sgen,
                       void *kwargs, stack_root *grandchild, int64_t order)
    :   stack(pos, nullptr, order), vec_kwargs_(vec_kwargs), 
        tgen_(tgen), sgen_(sgen), 
        grandchild_(grandchild)
{
    
}

stack_root::~stack_root()
{
    for (auto &child : children_)
        delete child.second;
}

edge* stack_root::__render(int32_t order, stack *ptr, bool next)
{
    if (!ptr) // when nullptr, default behavior
    {
        game::branch_type type = tgen_(1, kwargs_);

        auto branch = children_.find(1);
        if (branch != children_.end())
            return game::attach(type, this, branch->second);
    }
    else
    {
        game::branch_type type = tgen_(order+next-1, kwargs_);

        auto branch = children_.find(next ? order+1 : order-1);
        if (branch != children_.end())
            return game::attach(type, this, branch->second);
    }
    return nullptr; // one of the nodes are not created
}

void stack_root::operator() (node::container &nodes, const glm::vec3 &bottomleft, 
                             const glm::vec3 &topright, int32_t max_depth)
{
    int64_t first = sgen_.a_(bottomleft, topright, pos_, vec_kwargs_);
    if (first != NOT_FOUND) // if this entire stack is not in the region,
        return;             // then we don't need to do anything         

    // deal with grandchildren later
    if (grandchild_)
        return;

    for (int32_t child_ord = first; 
         child_ord < max_depth+first and (cap_==INF or child_ord < cap_);
         ++child_ord)
    {
        glm::vec3 child_pos = sgen_.a(child_ord, pos_, vec_kwargs_);
        if (IN(child_pos, bottomleft, topright))
        {
            auto *child_node = new stack(pos_, this, child_ord);
            children_[child_ord] = child_node;
            nodes.insert(child_node);
        }
    }
}

edge::container stack_root::render(int32_t max_breadth)
{
    edge::container edges;
    edge* next = root_->__render();
    if (next) edges.insert(next);
    return edges;
}

void stack_root::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
    os << "stack_root ";
    os << "@(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
    os << " with " << children_.size() << " generated children" ;
    if (layers)
    {
        os << std::endl;
        for (auto &child : children_)
        {
            os << std::endl;
            for (uint8_t i=0; i<=counter; ++i)
                os << "\t";
            child.second->log(os, layers-1, counter+1);
        }
    }
}

bool stack_root::attach(edge *e)
{
    if (grandchild_)
        return grandchild_->attach(e);
    
    if (e->p1 == this or e->p2 == this)
    {
        grandchild_ = e->get_other(this);
        return true;
    }
    return false;
}

void stack_root::detach(edge *e)
{
    if (grandchild_)
        grandchild_->detach(e);
    // not done :(((
}

void stack_root::detach(int64_t order)
{
    // kill off everything greater with order greater than order
    
    cap_ = order; // not sure if i need a -1 here
    
    for (auto it = children_.find(order); it != children_.end(); ++it)
    {
        delete it->second;
        children_.erase(it);
    }
}