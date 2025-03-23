#ifndef KDTREE_HH
#define KDTREE_HH

#include <QVector>

#include "types/vec2.hh"

class KDNode {
public:
    KDNode(const Vec2 &point, KDNode *parent);
    KDNode(const Vec2 &point);
    ~KDNode();

    KDNode *insert(const Vec2 &point);

    const Vec2 &point() const {
        return point_;
    }

    KDNode *left() {
        return left_;
    }

    KDNode *right() {
        return right_;
    }

    KDNode *parent() {
        return parent_;
    }

    static const int DIMS = 2;

private:
    KDNode *insert_(const Vec2 &point, int k);

    Vec2 point_;
    KDNode *left_;
    KDNode *right_;
    KDNode *parent_;
};

class KDTree {
public:
    KDTree();
    KDTree(const QVector<Vec2> &points);
    ~KDTree();

    KDNode *insert(const Vec2 &point);

    KDNode *nearestNeighbor(const Vec2 &target);
    QVector<KDNode *> nearestNeighbors(const Vec2 &target, float radius);

private:
    KDNode *nearestNeighbor_(KDNode *root, const Vec2 &target, int depth);
    void nearestNeighbors_(KDNode *root, const Vec2 &target, float radius, QVector<KDNode *> &result, int depth);
    static KDNode *closest_(KDNode *node1, KDNode *node2, const Vec2 &target);

    KDNode *root_;
};

#endif