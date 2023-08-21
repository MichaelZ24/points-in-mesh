#include "main.hpp"

// Given a range of points, checks which ones are inside the mesh.
void checkPointsInRange(const std::vector<Point>& points, 
                        const std::vector<Triangle>& triangles, 
                        int startIdx, 
                        int endIdx, 
                        std::vector<Point>& insidePoints) {
    for (int i = startIdx; i < endIdx; ++i) {
        if (isPointInsideMesh(points[i], triangles)) {
            insidePoints.push_back(points[i]);
        }
    }
}

int main() {

    // Set Filenames
    const std::string pointsFilename = "data/pts_medium.txt";
    const std::string objFilename = "data/mesh_dense.obj";

    // Check Directories
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    std::cout << "Attempting to open: " << std::filesystem::absolute(pointsFilename) << std::endl;

    // Load points and triangles
    std::vector<Point> points = loadPoints(pointsFilename);
    std::vector<Triangle> triangles = loadOBJ(objFilename);

    // Basic statistics of data
    std::cout << "Loaded " << points.size() << " points from " << pointsFilename << std::endl;
    std::cout << "Loaded " << triangles.size() << " triangles from " << objFilename << std::endl;

    // Time the execution
    auto start = std::chrono::high_resolution_clock::now();

    // Setup multithreading
    int numThreads = std::thread::hardware_concurrency();  // This gets the number of CPU cores
    std::vector<std::thread> threads(numThreads);
    std::vector<std::vector<Point>> results(numThreads);

    int pointsPerThread = points.size() / numThreads;
    for (int i = 0; i < numThreads; ++i) {
        int startIdx = i * pointsPerThread;
        int endIdx = (i == numThreads - 1) ? points.size() : startIdx + pointsPerThread;
        threads[i] = std::thread(checkPointsInRange, std::ref(points), std::ref(triangles), startIdx, endIdx, std::ref(results[i]));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Collect results
    std::vector<Point> insidePoints;
    for (const auto& res : results) {
        insidePoints.insert(insidePoints.end(), res.begin(), res.end());
    }

    // Get execution duration
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Print out number of inside points and execution time
    std::cout << "Total points inside the mesh: " << insidePoints.size() << std::endl;
    std::cout << "Time taken: " << duration.count() << "ms" << std::endl;

    // Save new obj with mesh and inside/outside points for visualization
    std::vector<Point> outsidePoints = computeOutsidePoints(points, insidePoints);
    saveDataToOBJ(insidePoints, outsidePoints, triangles, "visualization.obj");

    // Save the inside points to a text file
    savePointsToFile(insidePoints, "insidePoints.txt");
    
    return 0;
}
