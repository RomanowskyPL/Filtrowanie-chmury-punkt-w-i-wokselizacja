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