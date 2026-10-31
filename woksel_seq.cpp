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

int main() {
    const int NUM_POINTS = 5000000;
    const float VOXEL_SIZE = 2.0f;

    std::cout << "Generowanie punktow...\n";
    std::vector<Point3D> cloud = generate_point_cloud(NUM_POINTS);

    std::cout << "Grupowanie sekwencyjne...\n";
    
    auto start_time = std::chrono::high_resolution_clock::now();

    std::unordered_map<VoxelKey, VoxelData, VoxelKeyHash> voxel_grid;

    for(const auto& p : cloud) {
        VoxelKey key = get_voxel_key(p, VOXEL_SIZE);

        voxel_grid[key].sum.x += p.x;
        voxel_grid[key].sum.y += p.y;
        voxel_grid[key].sum.z += p.z;
        voxel_grid[key].count++;
    }

    std::vector<Point3D> simplified_cloud;
    simplified_cloud.reserve(voxel_grid.size());

    for(const auto& pair : voxel_grid) {
        float avg_x = pair.second.sum.x / pair.second.count;
        float avg_y = pair.second.sum.y / pair.second.count;
        float avg_z = pair.second.sum.z / pair.second.count;
        
        simplified_cloud.push_back({avg_x, avg_y, avg_z});
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "Gotowe. Czas wykonania: " << elapsed.count() << " s\n";
    std::cout << "Liczba punktów wejsciowych: " << cloud.size() << "\n";
    std::cout << "Liczba wokseli (zredukowana chmura) " << simplified_cloud.size() << "\n";

    return 0;
}