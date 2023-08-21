#include "raycasting.hpp"

Point normalize(const Point& p) {
    float magnitude = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    return {p.x / magnitude, p.y / magnitude, p.z / magnitude};
}


bool rayIntersectsTriangle(const Point& rayOrigin, const Point& rayDirection, const Triangle& tri) {
    const float EPSILON = 0.0000001;
    Point edge1, edge2, h, s, q;
    float a, f, u, v;

    edge1.x = tri.v2.x - tri.v1.x;
    edge1.y = tri.v2.y - tri.v1.y;
    edge1.z = tri.v2.z - tri.v1.z;

    edge2.x = tri.v3.x - tri.v1.x;
    edge2.y = tri.v3.y - tri.v1.y;
    edge2.z = tri.v3.z - tri.v1.z;

    h.x = rayDirection.y * edge2.z - rayDirection.z * edge2.y;
    h.y = rayDirection.z * edge2.x - rayDirection.x * edge2.z;
    h.z = rayDirection.x * edge2.y - rayDirection.y * edge2.x;

    a = edge1.x * h.x + edge1.y * h.y + edge1.z * h.z;

    if (a > -EPSILON && a < EPSILON) {
        return false;
    }

    f = 1.0 / a;
    s.x = rayOrigin.x - tri.v1.x;
    s.y = rayOrigin.y - tri.v1.y;
    s.z = rayOrigin.z - tri.v1.z;

    u = f * (s.x * h.x + s.y * h.y + s.z * h.z);
    if (u < 0.0 || u > 1.0) {
        return false;
    }

    q.x = s.y * edge1.z - s.z * edge1.y;
    q.y = s.z * edge1.x - s.x * edge1.z;
    q.z = s.x * edge1.y - s.y * edge1.x;

    v = f * (rayDirection.x * q.x + rayDirection.y * q.y + rayDirection.z * q.z);
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }

    float t = f * (edge2.x * q.x + edge2.y * q.y + edge2.z * q.z);
    if (t > EPSILON) {
        return true;
    } else {
        return false;
    }
}


bool isPointInsideMesh(const Point& p, const std::vector<Triangle>& triangles) {
    Point rayDirection = { 1.0f, 0.0f, 0.0f }; // Already normalized

    AABB globalBox = { { FLT_MAX, FLT_MAX, FLT_MAX }, { -FLT_MAX, -FLT_MAX, -FLT_MAX } };

    // Efficiently compute the global bounding box
    for (const Triangle& tri : triangles) {
        for (int i = 0; i < 3; i++) {
            const Point& vertex = (&tri.v1)[i]; // More readable indexing
            globalBox.min.x = std::min(globalBox.min.x, vertex.x);
            globalBox.min.y = std::min(globalBox.min.y, vertex.y);
            globalBox.min.z = std::min(globalBox.min.z, vertex.z);
            globalBox.max.x = std::max(globalBox.max.x, vertex.x);
            globalBox.max.y = std::max(globalBox.max.y, vertex.y);
            globalBox.max.z = std::max(globalBox.max.z, vertex.z);
        }
    }

    if (!rayIntersectsAABB(p, rayDirection, globalBox)) {
        return false;
    }

    std::atomic<int> atomicIntersections(0); 
    const int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; t++) {
        threads.push_back(std::thread([&](int start, int end) {
            int localIntersections = 0;
            for (int i = start; i < end; i++) {
                AABB triangleBox = { triangles[i].v1, triangles[i].v1 }; // Init with first vertex

                for (int j = 1; j < 3; j++) {
                    const Point& vertex = (&triangles[i].v1)[j]; // More readable indexing
                    triangleBox.min.x = std::min(triangleBox.min.x, vertex.x);
                    triangleBox.min.y = std::min(triangleBox.min.y, vertex.y);
                    triangleBox.min.z = std::min(triangleBox.min.z, vertex.z);
                    triangleBox.max.x = std::max(triangleBox.max.x, vertex.x);
                    triangleBox.max.y = std::max(triangleBox.max.y, vertex.y);
                    triangleBox.max.z = std::max(triangleBox.max.z, vertex.z);
                }

                if (rayIntersectsAABB(p, rayDirection, triangleBox) && rayIntersectsTriangle(p, rayDirection, triangles[i])) {
                    localIntersections++;
                }
            }
            atomicIntersections += localIntersections;
        }, t * triangles.size() / numThreads, (t + 1) * triangles.size() / numThreads));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return atomicIntersections.load() % 2 == 1;
}


