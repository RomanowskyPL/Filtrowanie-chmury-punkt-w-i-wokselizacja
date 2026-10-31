#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <random>
#include <chrono>

struct Point3D {
    float x, y, z;
};

struct VoxelKey {
    int x, y, z;

    bool operator == (const VoxelKey& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct VoxelKeyHash {
    std::size_t operator() (const VoxelKey&  k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1) ^ (std::hash<int>()(k.z) << 2);
    }
};

struct VoxelData {
    Point3D sum = {0.0f, 0.0f, 0.0f};
    int count = 0;
};

inline VoxelKey get_voxel_key(const Point3D& p, float voxel_size) {
    return {
        static_cast<int>(std::floor(p.x / voxel_size)),
        static_cast<int>(std::floor(p.y / voxel_size)),
        static_cast<int>(std::floor(p.z / voxel_size))
    };
}

std::vector<Point3D> generate_point_cloud(int num_points) {
    std::vector<Point3D> cloud(num_points);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-100.0f, 100.0f);

    for(int i = 0; i < num_points; ++i) {
        cloud[i] = {dist(gen), dist(gen), dist(gen)};
    }
    return cloud;
}
