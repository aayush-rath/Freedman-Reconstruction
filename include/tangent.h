#pragma once
#include "utils.h"
#include "kdTree.h"

/*
Approximating Tangent Space

- Tangent Basis: For a given point x in a K-dimensional
Space, the tangent basis is the set of K+1 points drawn 
from the neighborhood of x anc no other point in the 
neighborhood of x is enclosed by the K+1 points

- Enclosure: x is said to be enclosed by K+1 points if 
the projection of x on the hyperplane formed by the K+1 
points is inside the simplex formed by the K+1 points

- ATS(x): If the tangent basis is shifted to point x, it
is called the approximating tangent basis
*/


/*
Progress:
-09/01/2026: Implemented ATS

*/


template<int D>
using Vec = Point<D>;

template <int D>
class TangentBasis : public Vec{
public:
    KDTree<D> kdt;
    std::vector<Point<D>> point_set
    std::vector<Vec<D>> tangent_basis;

    TangentBasis(const std::vector<Point<D>>& point_set, const KDTree& kdt) : point_set(point_set), kdt(kdt) {
        computeTB();
    }
private:

    void computeTB() {
        int num_points = point_set.size();
        for (int i  = 0; i < num_points; i++) {
            std::vector<int> neighbors;
            kdt.knnSearch(point_set[i], D+1, neighbors);
            Point<D> x0 = point_set[neighbors[0]];

            Eigen::MatrixXf Phi(D, D);
            for (int j = 1; j <= D; j++) {
                for (int k = 1; k <= D; k++) {
                    Phi(j-1, k-1) = dot(point_set[neighbors[j]] - x0, point_set[neighbors[k]] - x0);
                }
            }

            Eigen::VectorXf phi(D);
            for (int j = 1; j <= D; j++) {
                phi(j-1) = 0.5(dot(point_set[neighbors[j]] - x0, point_set[i] - x0) + dot(point_set[i] - x0, point_set[neighbors[j]] - x0));
            }

            Eigen::VectorXf lambda_star = Phi.inverse() * phi;
            for (int j = 0; j < D; j++) {
                tangent_basis[i][j] = lambda_star(j);
            }
        }
    }
};

