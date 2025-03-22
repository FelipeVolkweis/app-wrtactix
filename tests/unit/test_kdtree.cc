#include "utils/testsuite/testsuite.hh"
#include "algorithm/geometry/kdtree/kdtree.hh"

class TestKDTree : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;
private slots:
    void testEmptyTree();
    void testSingleNode();
    void testNearest();
    void testBacktracking();
    void testExactMatch();
};

void TestKDTree::testEmptyTree() {
    KDTree emptyTree;
    KDNode* nn = emptyTree.nearestNeighbor(Vec2(0, 0));
    QVERIFY(nn == nullptr);
}

void TestKDTree::testSingleNode() {
    KDTree singleNodeTree;
    singleNodeTree.insert(Vec2(5, 5));
    KDNode* nn = singleNodeTree.nearestNeighbor(Vec2(0, 0));
    QVERIFY(nn != nullptr);
    QCOMPARE(nn->point(), Vec2(5, 5));
}

void TestKDTree::testNearest() {
    KDTree tree;
    tree.insert(Vec2(0, 0));
    tree.insert(Vec2(3, 0));
    
    KDNode* nn = tree.nearestNeighbor(Vec2(2, 0));
    QVERIFY(nn != nullptr);
    QCOMPARE(nn->point(), Vec2(3, 0));  // Closer to (3,0)
}

void TestKDTree::testBacktracking() {
    KDTree tree;
    tree.insert(Vec2(5, 5));  // Root
    tree.insert(Vec2(2, 5));  // Left subtree
    tree.insert(Vec2(8, 5));  // Right subtree
    
    KDNode* nn = tree.nearestNeighbor(Vec2(3, 5));
    QVERIFY(nn != nullptr);
    QCOMPARE(nn->point(), Vec2(2, 5));  // Should find in left subtree
}

void TestKDTree::testExactMatch() {
    KDTree tree;
    tree.insert(Vec2(1, 1));
    tree.insert(Vec2(2, 2));
    tree.insert(Vec2(3, 3));
    
    KDNode* nn = tree.nearestNeighbor(Vec2(2, 2));
    QVERIFY(nn != nullptr);
    QCOMPARE(nn->point(), Vec2(2, 2));  // Exact match
}

static TestKDTree TEST_KDTREE;
#include "test_kdtree.moc"