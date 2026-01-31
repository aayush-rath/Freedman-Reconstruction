#pragma once
#include "utils.h"
#include <limits>
#include <algorithm>

/*
KD-Tree implementation for fast neighbor search

This is a classic implementation of a KDTree where one
sorts the points in a binary search tree structure of 
fast nearest neighbor search 
*/

template<int D>
class KDTree{
public:
    KDTree(const std::vector<Point<D>>& point_set) {
        pts = point_set;
        indices.resize(pts.size());
        for (size_t i = 0; i < pts.size(); i++) indices[i] = i;
        root = build(0, pts.size(), 0);
    }

    ~KDTree() {freeNode(root);}

    // Radius search
    void radiusSearch(const Point<D>& q,
                      double radius,
                      std::vector<int>& out_indices) const {
        double r2 = radius * radius;
        radiusSearchRec(root, q, r2, out_indices);
    }

    // Nearest neighbor search
    void knnSearch(const Point<D>& q,
                   int k,
                   std::vector<int>& out_indices) const {
        out_indices.clear();
        knnSearchRec(root, q, k, out_indices);
    }

private:
    // Binary tree node to store left and right node points and the index
    struct Node {
        int index;
        int axis;
        Node* left;
        Node* right;
    };

    std::vector<Point<D>> pts;
    std::vector<int> indices;
    Node* root = nullptr;

    // Recursive function to build a binary search tree
    Node* build(size_t l, size_t r, int depth) {

        if (l >= r) return nullptr;

        int axis = depth % D;
        size_t m = (l+r) / 2;
        std::nth_element(
            indices.begin() + l,
            indices.begin() + m,
            indices.begin() + r,
            [&](int a, int b) { return pts[a][axis] < pts[b][axis]; }
        );

        Node* node = new Node{
            indices[m],
            axis,
            build(l, m, depth+1),
            build(m+1, r, depth+1)
        };

        return node;
    }

    // Deletion of the binary search tree
    void freeNode(Node* n) {
        if (!n) return;
        freeNode(n->left);
        freeNode(n->right);
        delete n;
    }


    void radiusSearchRec(
        Node* node,
        const Point<D>& q, 
        double r2,
        std::vector<int>& out
    ) const {
        if (!node) return;

        const Point<D>& p = pts[node->index];
        if (sqdist(p, q) <= r2) out.push_back(node->index);
        int ax = node->axis;
        double diff = q[ax] - p[ax];

        Node* near = diff < 0 ? node->left : node->right;
        Node* far = diff < 0 ? node->right : node->left;

        radiusSearchRec(near, q, r2, out);
        if (diff * diff <= r2) radiusSearchRec(far, q, r2, out);
    }

    void knnSearchRec(Node* node,
                      const Point<D>& q,
                      int k,
                      std::vector<int>& best) const
    {
        if (!node) return;

        best.push_back(node->index);

        std::nth_element(
            best.begin(),
            best.begin() + std::min(k, (int)best.size()),
            best.end(),
            [&](int a, int b) {
                return sqdist(pts[a], q) < sqdist(pts[b], q);
            });

        if ((int)best.size() > k)
            best.resize(k);

        int ax = node->axis;
        double diff = q[ax] - pts[node->index][ax];

        Node* near = diff < 0 ? node->left : node->right;
        Node* far  = diff < 0 ? node->right : node->left;

        knnSearchRec(near, q, k, best);

        if ((int)best.size() < k ||
            diff * diff < sqdist(pts[best.back()], q))
            knnSearchRec(far, q, k, best);
    }
};

