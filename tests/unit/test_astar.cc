#include "utils/testsuite/testsuite.hh"
#include "algorithm/pathplanner/astar/astar.hh"

class TestAStar : public TestSuite {
    Q_OBJECT
public:
    using TestSuite::TestSuite;
private slots:
    void testGrid();
    void testPath();
};

void TestAStar::testGrid() {
    AStarGrid<900, 600, 100> grid;
    QVERIFY(grid.width() == 900);
    QVERIFY(grid.height() == 600);
    QVERIFY(!grid.isObstacle(0, 0));
    QVERIFY(!grid.isObstacle(899, 599));
    Vec2 pos(0, 0);
    auto converted = grid.convertVec2ToGrid(pos);
    QVERIFY(converted.first == 450);
    QVERIFY(converted.second == 300);

    Vec2 convertBack = grid.convertGridToVec2(450, 300);
    QVERIFY(convertBack == pos);

    grid.setObstacle(pos, 0.09); // (0, 0) -> 450, 300 
    QVERIFY(grid.isObstacle(450, 300));
    QVERIFY(grid.isObstacle(451, 300));
    QVERIFY(grid.isObstacle(449, 300));
    QVERIFY(!grid.isObstacle(600, 300));
    grid.clearGrid();
    QVERIFY(!grid.isObstacle(0, 0));
    QVERIFY(!grid.isObstacle(899, 599));
}

void TestAStar::testPath() {
    // AStar astar;
    // Vec2 origin(0, 0);
    // Vec2 goal(1, 1);
    // QVector<Vec2> obstacles;
    // auto path = astar.findPath(origin, goal, obstacles);
    // QVERIFY(path.size() == 2);
    // QVERIFY(path[0] == origin);
    // QVERIFY(path[1] == goal);

    // Vec2 o1(0.5, 0.5);
    // Vec2 o2(0.5, 0.6);
    // Vec2 o3(0.5, 0.7);

    // obstacles.push_back(o1);
    // obstacles.push_back(o2);
    // obstacles.push_back(o3);

    // // path = astar.findPath(origin, goal, obstacles);
    
}

static TestAStar TEST_ASTAR;
#include "test_astar.moc"