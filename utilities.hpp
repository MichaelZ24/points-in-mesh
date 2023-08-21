#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>

/** Represents a 3D point with x, y, and z coordinates. */
struct Point {
    float x, y, z;

    /** Check if two points are equal.
     *  @param other The point to compare against.
     *  @return True if the points are equal, false otherwise.
     */
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

/** Hash function for the Point struct. */
struct PointHash {
    /** Compute a hash value for a given point.
     *  @param p The point to hash.
     *  @return The hash value.
     */
    size_t operator()(const Point& p) const {
        return std::hash<float>()(p.x) ^ std::hash<float>()(p.y) ^ std::hash<float>()(p.z);
    }
};

/** Represents a triangle composed of three vertices. */
struct Triangle {
    Point v1, v2, v3; // Vertices of the triangle
};

/** Load points from a text file.
 *  @param filename The path to the file containing points.
 *  @return A vector of loaded points.
 */
std::vector<Point> loadPoints(const std::string& filename);

/** Load triangles from an OBJ file.
 *  @param filename The path to the OBJ file.
 *  @return A vector of loaded triangles.
 */
std::vector<Triangle> loadOBJ(const std::string& filename);

/** Save points and triangles to an OBJ file.
 *  @param insidePoints Points inside the mesh.
 *  @param outsidePoints Points outside the mesh.
 *  @param triangles Triangles composing the mesh.
 *  @param filename The path of the output OBJ file.
 */
void saveDataToOBJ(const std::vector<Point>& insidePoints, 
                   const std::vector<Point>& outsidePoints, 
                   const std::vector<Triangle>& triangles, 
                   const std::string& filename);

/** Compute points outside the mesh.
 *  @param points All input points.
 *  @param insidePoints Points inside the mesh.
 *  @return Points outside the mesh.
 */
std::vector<Point> computeOutsidePoints(const std::vector<Point>& points, 
                                        const std::vector<Point>& insidePoints);


/** Save points to a text file.
 *  @param points The points to be saved.
 *  @param filename The path of the output text file.
 */
void savePointsToFile(const std::vector<Point>& points, const std::string& filename);

#endif // UTILITIES_HPP
