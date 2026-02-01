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

template<int D>
class TangentBasis {
public:
    using Vec = Point<D>;

    std::vector<std::array<Vec, D>> bases;


    TangentBasis(
        const std::vector<Vec>& points,
        const KDTree<D>& kdt
    ) {
        compute(points, kdt);
    }

private:
    void compute(const std::vector<Vec>& points,
                 const KDTree<D>& kdt)
    {
        int N = points.size();
        bases.resize(N);
        for (int i = 0; i < N; ++i) {
            std::vector<int> nn;
            kdt.knnSearch(points[i], D + 1, nn);

            Eigen::MatrixXd V(D, D);
            for (int j = 1; j <= D; ++j)
                V.row(j-1) = (points[nn[j]] - points[nn[0]]).eigen();

            Eigen::JacobiSVD<Eigen::MatrixXd> svd(
                V, Eigen::ComputeThinV
            );

            for (int k = 0; k < D; ++k)
                bases[i][k] = Vec(svd.matrixV().col(k));
        }
    }
};
