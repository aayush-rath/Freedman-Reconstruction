#pragma once
#include <vector>
#include <cassert>
#include <random>
#include <iostream>
#include <Eigen/Dense>


// Dimension agnostic point representation
template<int D>
class Point {
public:
    Point() = default;
    Point(const double* point) : coords() {
        for (int i = 0; i < D; i++) {
            coords[i] = *(point+i);
        }
    }
    Point(const std::vector<double>& point) {
        assert(point.size() == D);
        for (int i = 0; i < D; i++) {
            coords[i] = point[i];
        }
    }

    double operator[](int i) const {
        return coords[i];
    }

    double& operator[](int i) {
        return coords[i];
    }
private:
    double coords[D];
};


// Point/Vector Operations
template<int D>
inline Point<D> operator+(const Point<D>& p, const Point<D>& q) {
    Point<D> sum;
    for (int i = 0; i < D; i++) {
        sum[i] = p[i] + q[i];
    }

    return sum;
}

template<int D>
inline Point<D> operator-(const Point<D>& p, const Point<D>& q) {
    Point<D> sub;
    for (int i = 0; i < D; i++) {
        sub[i] = p[i] - q[i];
    }

    return sub;
}


template<int D>
inline double dot(const Point<D>& p, const Point<D>& q) {
    double prod = 0.0;
    for (int i = 0; i < D; i++) {
        prod += p[i] * q[i];
    }

    return prod;
}


template<int D>
inline double sqdist(const Point<D>& p, const Point<D>& q) {
    double dist = 0.0;
    for (int i = 0; i < D; i++) {
        dist += (p[i] - q[i]) * (p[i] - q[i]);
    }

    return dist;
}


// Random point generator using a uniform distribution with space bounds
inline double random_U() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dist(0.0,1.0);
    return dist(gen);
}

template<int D>
inline Point<D> random_point(double lower_bound, double upper_bound) {
    Point<D> point;
    for (int i = 0; i < D; i++) point[i] = lower_bound + random_U() * (upper_bound - lower_bound);

    return point;
}

// Printing the coordinates of a point
template<int D>
inline std::ostream& operator<<(std::ostream& out, const Point<D>& p) {
    for (int i = 0; i < D; i++) out << p[i];
    return out;
}

// Eigenification of a Point for linear algebra
template<int D>
Eigen::VectorXf eigenify(const Point<D>& point) {
    Eigen::VectorXf eig_point(D);
    for (int i = 0; i < point.size(); i++) eig_point(i) = point[i];
}