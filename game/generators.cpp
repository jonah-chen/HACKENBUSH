#include "nodes.hpp"

#define FLOAT_EPSILON 1e-7f

static inline glm::vec3 operator*(const glm::vec3 &v, float m)
{
	return glm::vec3(v.x * m, v.y * m, v.z * m);
}

/**
 * @brief determine when a ray {R=At+B, t>=0} intersects with a box.
 * 
 * @pre each of the three coordinates of bottomleft must be smaller than the
 * corresponding coordinate of topright.
 * 
 * @param A vec3 describing tangent vector to the ray.
 * @param B vec3 describing the initial point of the ray.
 * @param bottomleft vec3 describing the bottom left corner of the box.
 * @param topright vec3 describing the top right corner of the box.
 * @return float of the time when the ray intersects with the box.
 * @return -1 if the ray does not intersect with the box.
 * @return 0 if the ray is already inside the box at t=0.
 * 
 * @details compute when each of the 3 components of the line intersects each
 * of the 3 components of the bottomleft and topright corners. Taking the 
 * smaller of these times, we get the earliest time of intersection of each 
 * component (and vise versa for the larger). We know the line will start
 * intersecting the box at the latest of the earliest times of intersection for 
 * each coordinate (and vise versa for when the line stop intersecting the box).
 * 
 * We know if the earliest time when the line intersects the box is later than 
 * the latest time the line intersects the box, something is wrong and the line
 * does not intersect the box. Also, if the earliest time is negative, the line 
 * is either already inside the box or it has already intersected the box.
 */
static float intersect(const glm::vec3 &A, const glm::vec3 &B,
					   const glm::vec3 &bottomleft, const glm::vec3 &topright)
{
	// compute when the line At+B intersects the x boundaries of the box
	float tx1 = (bottomleft.x - B.x) / (A.x + FLOAT_EPSILON);
	float tx2 = (topright.x - B.x) / (A.x + FLOAT_EPSILON);

	// compute when the line At+B intersects the y boundaries of the box
	float ty1 = (bottomleft.y - B.y) / (A.y + FLOAT_EPSILON);
	float ty2 = (topright.y - B.y) / (A.y + FLOAT_EPSILON);

	// compute when the line At+B intersects the z boundaries of the box
	float tz1 = (bottomleft.z - B.z) / (A.z + FLOAT_EPSILON);
	float tz2 = (topright.z - B.z) / (A.z + FLOAT_EPSILON);

	// find the earliest and latest times of intersection
	float tmin = std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)),
						  std::min(tz1, tz2));
	float tmax = std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)),
						  std::max(tz1, tz2));

	// if tmax < 0, the line is intersecting the box in the past
	// if tmin > tmax, the line is not intersecting the box
	// build in a small epsilon to account for floating point error
	if (tmax < -1e-8 or tmax <= tmin) return -1;
	// if the line does intersect the box but is already inside, then return 0
	if (tmin < 0) return 0;
	// otherwise, return the earliest time of intersection
	return tmin;
}


namespace game::nodes::generators {

////////////////////////////////////////////////////////////////////////////////
// Collection of implemented branch generators
namespace F {

branch_type red(const int64_t order, void *kwargs)
{ return game::red; }

branch_type green(const int64_t order, void *kwargs)
{ return game::green; }

branch_type blue(const int64_t order, void *kwargs)
{ return game::blue; }

static branch_type fraction(const int64_t order, void *kwargs, branch_type one,
					 branch_type zero)
{
	int32_t numerator = ((int32_t *) kwargs)[0];
	uint32_t denominator = ((uint32_t *) (kwargs))[1];
	assert(numerator and denominator);
	assert(denominator & (denominator - 1));

	// compute the fraction
	// don't want to deal with negative numbers. Thus, we use the absolute value
	if (numerator < 0)
	{
		branch_type tmp = one;
		one = zero;
		zero = tmp;
		numerator = -numerator;
	}

	uint32_t integral_part = numerator / denominator;

	int64_t fractional_order = order - integral_part;
	if (fractional_order < 0)
		return one;

	uint32_t fractional_part = numerator % denominator;
	auto frac = std::make_pair(numerator % denominator, denominator);
	auto it = stack_root::fraction_lut.find(frac);

	// if it is not in the LUT, compute it and insert it
	if (it != stack_root::fraction_lut.end())
	{
		auto &bits = it->second;
		return bits[fractional_order % bits.size()] ? one : zero;
	}
	else
	{
		std::vector<bool> bits;
		while(true)
		{
			fractional_part <<= 1;
			bits.push_back(fractional_part >= denominator);
			fractional_part %= denominator;
			const std::size_t length = bits.size();
			if (!(length & 1))
			{
				auto begining = bits.begin();
				auto half_way = begining + length / 2;
				if (std::equal(begining, half_way, half_way))
				{
					bits.erase(half_way, bits.end());
					break;
				}
			}
		}
		// add bits to lut
		stack_root::fraction_lut[frac] = bits;
		return bits[fractional_order % bits.size()] ? one : zero;
	}
}
	/**
	 * @brief
	 *
	 * @pre Numerator and denominator must be nonzero.
	 * @pre denominator must be positive, and not a power of 2.
	 *
	 * @param order
	 * @param kwargs
	 * @return
	 */
	// kwargs is 2 int32_t's. Meaning it must be 8 bytes.
branch_type fraction(const int64_t order, void *kwargs)
{
	if (kwargs)
		return fraction(order, kwargs, game::red, game::blue);
	throw std::runtime_error("Fractional generator requires two int32_t's as kwargs");
}

}


glm::vec3 f::geometric(const int64_t order, const glm::vec3 &rootpos,
					   const glm::vec3 &kwargs)
{
	if (order == INF) return rootpos + kwargs;
	return rootpos +
		   kwargs * (1.0f - std::pow(GEOMETRIC_CONSTANT, (float) order));
}

int64_t f_::geometric(const glm::vec3 &bottomleft, const glm::vec3 &topright,
					  const glm::vec3 &rootpos, const glm::vec3 &kwargs)
{
	// B is the root position
	// A is the direction (kwargs)

	float t = intersect(kwargs, rootpos, bottomleft, topright);

	// if the line does not intersect the box formed by the infinite stack, or
	// it only intersects at a time greater than the limit (which is 1), then
	// return NOT_FOUND
	if (t < 0 or t > 1) return NOT_FOUND;

	// otherwise, return the step when it first intersects.
	return (int64_t) (std::log2(1.0f - t) / std::log2(GEOMETRIC_CONSTANT));
}
}