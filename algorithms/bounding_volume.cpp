#include "bounding_volume.hpp"

bool rayIntersectsAABB(const Point& rayOrigin, const Point& rayDirection, const AABB& box) {
    float tmin = (box.min.x - rayOrigin.x) / rayDirection.x;
    float tmax = (box.max.x - rayOrigin.x) / rayDirection.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    float tymin = (box.min.y - rayOrigin.y) / rayDirection.y;
    float tymax = (box.max.y - rayOrigin.y) / rayDirection.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    float tzmin = (box.min.z - rayOrigin.z) / rayDirection.z;
    float tzmax = (box.max.z - rayOrigin.z) / rayDirection.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;
        
    return true;
}
