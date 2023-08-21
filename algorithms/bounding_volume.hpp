#ifndef BOUNDING_VOLUME_HPP
#define BOUNDING_VOLUME_HPP

#include "../utilities.hpp"

/** Axis-Aligned Bounding Box (AABB) structure.
 *  An AABB is defined by its minimum and maximum corners.
 */
struct AABB {
    Point min; // minimum corner
    Point max; // maximum corner
};

/** Check if a ray intersects an Axis-Aligned Bounding Box (AABB).
 *  @param rayOrigin The origin of the ray.
 *  @param rayDirection The direction of the ray.
 *  @param box The AABB to test against.
 *  @return True if the ray intersects the AABB, false otherwise.
 */
bool rayIntersectsAABB(const Point& rayOrigin, const Point& rayDirection, const AABB& box);

#endif // BOUNDING_VOLUME_HPP
