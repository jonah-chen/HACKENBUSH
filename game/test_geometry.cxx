#include <glm/glm.hpp>
#include <iostream>

static inline glm::vec3 operator* (const glm::vec3& v, float m) {
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
    float tx1 = (bottomleft.x - B.x) / A.x;
    float tx2 = (topright.x - B.x) / A.x;

    // compute when the line At+B intersects the y boundaries of the box
    float ty1 = (bottomleft.y - B.y) / A.y;
    float ty2 = (topright.y - B.y) / A.y;

    // compute when the line At+B intersects the z boundaries of the box
    float tz1 = (bottomleft.z - B.z) / A.z;
    float tz2 = (topright.z - B.z) / A.z;

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

int main(int argc, char** argv)
{
    if (argc != 13)
        exit(1);
    
    glm::vec3 A(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]));
    glm::vec3 B(std::stof(argv[4]), std::stof(argv[5]), std::stof(argv[6]));
    glm::vec3 bottomleft(std::stof(argv[7]), std::stof(argv[8]), std::stof(argv[9]));
    glm::vec3 topright(std::stof(argv[10]), std::stof(argv[11]), std::stof(argv[12]));

    std::cout << "A: " << A.x << " " << A.y << " " << A.z << std::endl;
    std::cout << "B: " << B.x << " " << B.y << " " << B.z << std::endl;
    std::cout << "bottomleft: " << bottomleft.x << " " << bottomleft.y << " " << bottomleft.z << std::endl;
    std::cout << "topright: " << topright.x << " " << topright.y << " " << topright.z << std::endl;

    float t = intersect(A,B,bottomleft,topright);

    if (t == -1.0f)
        std::cout << "no intersection" << std::endl;
    else
    {
        std::cout << "At+B intersect at t=" << t;
        glm::vec3 x = A*t + B;
        std::cout << "@(" << x.x << "," << x.y << "," << x.z << ")" << std::endl;
    }
    return 0;
}