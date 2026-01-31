#include "utils.h"
#include "kdTree.h"
#include "tangent.h"
#include <chrono>

int main(int argc, char const *argv[]) {
    int num_points = 10000;

    std::vector<Point<3>> point_set;
    for (int i = 0; i < num_points; i++) {
        point_set.push_back(random_point<3>(-M_PI, M_PI));
    }

    auto start = std::chrono::high_resolution_clock::now();
    KDTree<3> kdt(point_set);
    std::vector<int> neighbors;
    kdt.knnSearch(point_set[100], 10, neighbors);
    auto end = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time for distance calculation: " << dur.count() << " milliseconds" << std::endl;
    return 0;
}
