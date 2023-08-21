#include "utilities.hpp"

std::vector<Point> loadPoints(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Point p;
        iss >> p.x >> p.y >> p.z;
        points.push_back(p);
    }

    file.close();
    return points;
}

// Not using library to comply with requirements. Something like tinyobjloader would be suitable otherwise.
std::vector<Triangle> loadOBJ(const std::string& filename) {
    std::vector<Triangle> triangles;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return triangles;
    }

    std::vector<Point> vertices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        char identifier;
        iss >> identifier;

        if (identifier == 'v') {
            Point v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } 
        else if (identifier == 'f') {
            int idx1, idx2, idx3; 
            iss >> idx1 >> idx2 >> idx3;
            
            idx1--; idx2--; idx3--;

            if(idx1 < vertices.size() && idx2 < vertices.size() && idx3 < vertices.size()) {
                triangles.push_back({vertices[idx1], vertices[idx2], vertices[idx3]});
            } else {
                std::cerr << "Invalid vertex index in OBJ file." << std::endl;
            }
        }
    }

    file.close();
    return triangles;
}

void saveDataToOBJ(const std::vector<Point>& insidePoints, 
                   const std::vector<Point>& outsidePoints, 
                   const std::vector<Triangle>& triangles, 
                   const std::string& filename) 
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }

    file << "o InsidePoints\n";
    for (const auto& pt : insidePoints) {
        file << "v " << pt.x << " " << pt.y << " " << pt.z << "\n";
    }

    file << "o OutsidePoints\n";
    for (const auto& pt : outsidePoints) {
        file << "v " << pt.x << " " << pt.y << " " << pt.z << "\n";
    }

    file << "o Triangles\n";
    int offset = insidePoints.size() + outsidePoints.size();  // account for the point vertices
    for (const auto& tri : triangles) {
        file << "v " << tri.v1.x << " " << tri.v1.y << " " << tri.v1.z << "\n";
        file << "v " << tri.v2.x << " " << tri.v2.y << " " << tri.v2.z << "\n";
        file << "v " << tri.v3.x << " " << tri.v3.y << " " << tri.v3.z << "\n";

        file << "f " << offset + 1 << " " << offset + 2 << " " << offset + 3 << "\n";
        offset += 3;  // for the 3 vertices of the triangle
    }

    file.close();
}


std::vector<Point> computeOutsidePoints(const std::vector<Point>& points, 
                                        const std::vector<Point>& insidePoints) 
{
    std::vector<Point> outsidePoints;
    std::unordered_set<Point, PointHash> insidePointSet(insidePoints.begin(), insidePoints.end());

    for (const auto& point : points) {
        if (insidePointSet.find(point) == insidePointSet.end()) {
            outsidePoints.push_back(point);
        }
    }

    return outsidePoints;
}

void savePointsToFile(const std::vector<Point>& points, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& pt : points) {
        file << pt.x << " " << pt.y << " " << pt.z << "\n";
    }

    file.close();
    std::cout << "Points saved to " << filename << std::endl;
}

