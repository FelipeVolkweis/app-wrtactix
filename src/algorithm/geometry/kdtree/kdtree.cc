#include "kdtree.hh"

KDNode::KDNode(const Vec2& point, KDNode* parent) : point_(point), parent_(parent) {
    left_ = nullptr;
    right_ = nullptr;
}

KDNode::KDNode(const Vec2& point) : point_(point) {
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
}

KDNode::~KDNode() {
    delete left_;
    delete right_;
}

void KDNode::insert(const Vec2& point) {
    insert_(point, 0);
}

void KDNode::insert_(const Vec2& point, int axis) {
    if (point[axis] < point_[axis]) {
        if (left_ == nullptr) {
            left_ = new KDNode(point, this);
        } else {
            left_->insert_(point, (axis + 1) % DIMS);
        }
    } else {
        if (right_ == nullptr) {
            right_ = new KDNode(point, this);
        } else {
            right_->insert_(point, (axis + 1) % DIMS);
        }
    }
}

KDTree::KDTree() {
    root_ = nullptr;
}

KDTree::KDTree(const QVector<Vec2>& points) {
    root_ = nullptr;
    for (const Vec2& point : points) {
        insert(point);
    }
}

KDTree::~KDTree() {
    delete root_;
}

void KDTree::insert(const Vec2& point) {
    if (root_ == nullptr) {
        root_ = new KDNode(point, 0);
    } else {
        root_->insert(point);
    }
}

KDNode *KDTree::nearestNeighbor(const Vec2& target) {
    return nearestNeighbor_(root_, target, 0);
}

KDNode *KDTree::nearestNeighbor_(KDNode *root, const Vec2& target, int axis) {
    if (root == nullptr) return root;

    KDNode *nextBranchToLookInto = nullptr;
    KDNode *remainingBranch = nullptr;

    if (target[axis] < root->point()[axis]) {
        nextBranchToLookInto = root->left();
        remainingBranch = root->right();
    } else {
        nextBranchToLookInto = root->right();
        remainingBranch = root->left();
    }

    KDNode *temporaryNN = nearestNeighbor_(nextBranchToLookInto, target, (axis + 1) % KDNode::DIMS);
    KDNode *best = closest_(temporaryNN, root, target);

    float r = (target - best->point()).norm();
    float rPrime = target[axis] - root->point()[axis];

    if (r * r >= rPrime * rPrime) {
        temporaryNN = nearestNeighbor_(remainingBranch, target, (axis + 1) % KDNode::DIMS);
        best = closest_(temporaryNN, best, target);
    }

    return best;
}

KDNode *KDTree::closest_(KDNode *node1, KDNode *node2, const Vec2& target) {
    if (node1 == nullptr) {
        return node2;
    }

    if (node2 == nullptr) {
        return node1;
    }

    float d1 = (target - node1->point()).norm();
    float d2 = (target - node2->point()).norm();

    if (d1 < d2) {
        return node1;
    } 

    return node2;
}