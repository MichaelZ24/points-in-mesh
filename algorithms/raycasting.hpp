#ifndef RAYCASTING_HPP
#define RAYCASTING_HPP

#include "../utilities.hpp"
#include "bounding_volume.hpp"

#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include <cfloat>


/** Check if a ray intersects a triangle.
 *  @param rayOrigin The origin of the ray.
 *  @param rayDirection The direction of the ray.
 *  @param tri The triangle to test against.
 *  @return True if the ray intersects the triangle, false otherwise.
 */
bool rayIntersectsTriangle(const Point& rayOrigin, const Point& rayDirection, const Triangle& tri);

/** Check if a point is inside a mesh defined by triangles.
 *  @param p The point to test.
 *  @param triangles The list of triangles composing the mesh.
 *  @return True if the point is inside the mesh, false otherwise.
 */
bool isPointInsideMesh(const Point& p, const std::vector<Triangle>& triangles);

#endif // RAYCASTING_HPP
