#pragma once
#include "utils.h"

/*
Edge-set implementation
*/

#include <libqhullcpp/Qhull.h>
#include <libqhullcpp/QhullFacet.h>
#include <libqhullcpp/QhullFacetList.h>
#include <libqhullcpp/QhullVertex.h>
#include <libqhullcpp/QhullVertexSet.h>

template<int D>
struct EdgeSet {
    std::vector<std::vector<int>> facets;
};

template<int D>
EdgeSet<D> compute_conv_hull(const std::vector<Point<D>>& points) {
    const int num_points = points.size();

    // Lost my aura here
    std::vector<double> coords;
    coords.reserve(num_points * D);

    for (const auto& p : points) {
        for (int i = 0; i < D; i++) {
            coords.push_back(p[i]);
        }
    } 

    orgQhull::Qhull qhull;
    qhull.runQhull(
        "",
        D,
        num_points,
        coords.data(),
        "Qt"
    );

    EdgeSet<D> hull;

    const auto& facet_list = qhull.facetList();
    for (auto it = facet_list.begin(); it != facet_list.end(); it++) {
        if (!it->isGood()) continue;
        
        std::vector<int> facet_indices;
        const auto& vertices = it->vertices();

        for (auto v_it = vertices.begin(); v_it != vertices.end(); v_it++) {
            facet_indices.push_back((*v_it).point().id());
        }

        hull.facets.push_back(std::move(facet_indices));
    }

    return hull;
}

template<int D>
class IntersectionOfPBH {
public:
    std::vector<Point<D>> point_set;
    EdgeSet<D> edge_set;

private:
    Eigen::MatrixXd A,
    Eigen::MatrixXd b
};