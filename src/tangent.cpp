#include "tangent.h"

template<int D>
void getDistances(const std::vector<Point<D>>& point_set, std::vector<double>& distances) {
    int num_points = point_set.size();
    distances.reserve(num_points * num_points);
    for (int i = 0; i < num_points; i++) {
        for (int j = 0; j < num_points; j++) {
            if (i == j) {
                distances[i * num_points + j] = 0;
                continue;
            }
            distances[i * num_points + j] = sqdist<3>(point_set[i], point_set[j]);
        }
    }
}

template<int D>
std::vector<Point<D>> getNeighbors(
    const std::vector<Point<D>>& point_set,
    const std::vector<double>& distances,
    const int point_index,
    const int num_points
) {
    int total_points = point_set.size();
    std::vector<Point<D>> neighbors;
    std::vector<int> sorted_points(total_points);
    for (int i = 0; i < total_points; i++) sorted_points[i] = i;

    auto dist_sort = [&] (const int& p, const int& q) {
        return distances[point_index * total_points + p] < distances[point_index * total_points + q];
    };

    std::sort(sorted_points.begin(), sorted_points.end(), dist_sort);
    
    for (int i = 0; i < num_points; i++) {
        neighbors.push_back(point_set[sorted_points[i]]);
    }

    return neighbors;
}
