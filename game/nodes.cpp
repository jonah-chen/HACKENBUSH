#include "nodes.hpp"

/**
 * @pre each of the 3 coordinates of BOT_L must be less than the corresponding 
 * coordinate of TOP_R.
 * @param X a vec3 of the point of interest.
 * @param BOT_L a vec3 of the bottom left corner of the bounding box.
 * @param TOP_R a vec3 of the top right corner of the bounding box.
 * @return true if `X` is within the bounding box.
 * @return false if `X` is not within the bounding box.
 */
#define IN(X, BOT_L, TOP_R) ((X).x >= (BOT_L).x && (X).x <= (TOP_R).x && \
                           (X).y >= (BOT_L).y && (X).y <= (TOP_R).y && \
                           (X).z >= (BOT_L).z && (X).z <= (TOP_R).z)

///////////////////////////////////////////////////////////////////////////////
// Implementation of the normal nodes
///////////////////////////////////////////////////////////////////////////////

namespace game::nodes {

/**
 * @details use a recursive (depth first) traversal of the bree with a maximum
 * depth to search for the all the nodes attached to this node. 
 */
void normal::operator()(container &nodes, const glm::vec3 &bottomleft,
						const glm::vec3 &topright, int32_t max_depth)
{
	(IN(pos_, bottomleft, topright) ? nodes : nodes_discard).insert(this);

	if (max_depth)
	{
		for (edge *edge: edges_)
		{
			node *other = edge->get_other(this);
			if (nodes.find(other) == nodes.end() and nodes_discard.find
					(other) == nodes_discard.end())    //
				// when other node not
				// visited
				(*other)(nodes, bottomleft, topright, max_depth - 1);
		}
	}
	if (max_depth == DEFAULT_MAX_DEPTH)
		nodes_discard.clear();
}

// add the edges attached to this node using std::set_union
void normal::render(edge::container &edges, int32_t max_breadth)
{
	std::set_union(edges.begin(), edges.end(), edges_.begin(), edges_.end(),
				   std::inserter(edges, edges.begin()));
}

void normal::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
	if (layers > 5) // layers>5 is not allowed
		throw layers;

	os << "normal ";
	os << "@(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
	os << " with " << edges_.size() << " edges";
	if (layers)
	{
		os << " to";
		for (edge *edge: edges_)
		{
			os << std::endl;
			for (uint8_t i = 0; i <= counter; ++i)
				os << "\t";
			edge->get_other(this)->log(os, layers - 1, counter + 1);
		}
	}
}

// insert the edge and return true, since a finite number (1) of edges is always 
// allowed to be inserted. 
bool normal::attach(edge *e)
{
	edges_.insert(e);
	return true;
}

// set erase method automatically error checks
void normal::detach(edge *e)
{
	edges_.erase(e);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the stacked nodes
///////////////////////////////////////////////////////////////////////////////

void stack::operator()(node::container &nodes, const glm::vec3 &bottomleft,
					   const glm::vec3 &topright, int32_t max_depth)
{
	throw std::logic_error("not implemented because stack objects should not "
						   "be called!");
}

// return the edge that is BOTH moving away or towards the root
void stack::render(edge::container &edges, int32_t max_breadth)
{
	edge *prev = root_->__render(order_, this, false);
	edge *next = root_->__render(order_, this, true);
	if (prev) edges.insert(prev);
	if (next) edges.insert(next);
}

void stack::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
	os << "stack #" << order_;
	os << " @(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
}

bool stack::attach(edge *e)
{
	return true;
	// i don't think this should do anything?
}

void stack::detach(edge *e)
{
	stack *other = (stack *) (e->get_other(this));
	if (other->order_ < order_)
		root_->detach(order_);
}

///////////////////////////////////////////////////////////////////////////////
// Implementation of the stack root
///////////////////////////////////////////////////////////////////////////////

std::unordered_map<std::pair<int32_t, int32_t>, std::vector<bool>>
stack_root::fraction_lut;

stack_root::stack_root(const glm::vec3 &pos, const glm::vec3 &vec_kwargs,
					   generators::type_gen tgen, generators::step_gen sgen,
					   void *kwargs, int64_t order, int64_t cap)
		: stack(pos, nullptr, order),
		  kwargs_(kwargs), vec_kwargs_(vec_kwargs),
		  tgen_(tgen), sgen_(sgen), cap_(cap)
{
	const glm::vec3 end = sgen.a(INF, pos, vec_kwargs);
	if (end.x == std::numeric_limits<float>::quiet_NaN())
		grandchild_ = nullptr;
	else
		grandchild_ = new normal(end);

}

stack_root::~stack_root()
{
	for (auto &child: children_)
		delete child.second;
}

edge *stack_root::__render(int32_t order, stack *ptr, bool next)
{
	if (!ptr) // when nullptr, default behavior
	{
		game::branch_type type = tgen_(1, kwargs_);

		auto branch = children_.find(1);
		if (branch != children_.end())
			return game::attach(type, this, branch->second);
	} else
	{
		game::branch_type type = tgen_(order + next - 1, kwargs_);

		auto branch = children_.find(next ? order + 1 : order - 1);
		if (branch != children_.end())
			return game::attach(type, ptr, branch->second);
		// return game::attach(type, ptr, children_[next ? order+1 : order-1]);
	}
	return nullptr; // one of the nodes are not created
}

// will add the created object to the children, if it is not already there
stack *stack_root::operator[](std::size_t i)
{
	if (cap_ != INF and (int64_t) i >= cap_)
		return nullptr;

	auto branch = children_.find(i);
	if (branch != children_.end())
		return branch->second;

	glm::vec3 child_pos = sgen_.a(i, pos_, vec_kwargs_);
	stack *child_node = new stack(child_pos, this, i);

	children_[i] = child_node;
	return child_node;
}

void stack_root::operator()(node::container &nodes,
							const glm::vec3 &bottomleft,
							const glm::vec3 &topright, int32_t max_depth)
{
	int64_t first = sgen_.a_(bottomleft, topright, pos_, vec_kwargs_);
	if (first == NOT_FOUND) // if this entire stack is not in the region,
		return;             // then we don't need to do anything

	// deal with grandchildren later
	if (grandchild_)
	{
		max_depth /= 2;
		(*grandchild_)(nodes, bottomleft, topright, max_depth);
	}

	for (int32_t child_ord = first; child_ord < max_depth + first; ++child_ord)
	{
		glm::vec3 child_pos = sgen_.a(child_ord, pos_, vec_kwargs_);
		if (IN(child_pos, bottomleft, topright))
		{
			stack *child_node = (*this)[child_ord];
			if (child_node) nodes.insert(child_node);
			else return;
		}
	}
}

void stack_root::render(edge::container &edges, int32_t max_breadth)
{
	edge *next = root_->__render();
	if (next) edges.insert(next);
}

void stack_root::log(std::ostream &os, uint8_t layers, uint8_t counter) const
{
	os << "stack_root ";
	os << "@(" << pos_.x << "," << pos_.y << "," << pos_.z << ")";
	os << " with " << children_.size() << " generated children";
	if (layers)
	{
		os << std::endl;
		for (auto &child: children_)
		{
			os << std::endl;
			for (uint8_t i = 0; i <= counter; ++i)
				os << "\t";
			child.second->log(os, layers - 1, counter + 1);
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

// kill off everything greater with order greater than order
void stack_root::detach(int64_t order)
{
	// if the order is greater than the current order, do nothing
	// not sure if this check is necessary.
	if (cap_ != INF and order > cap_)
		return;

	cap_ = order; // not sure if i need a -1 here

	auto it = children_.find(order);
	for (auto j = children_.find(order); j != children_.end(); ++j)
		delete j->second;
	children_.erase(it, children_.end());
}

node *stack_root::get_grandchild() const
{
	return grandchild_;
}

}