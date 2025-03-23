#ifndef ASTARGRID_HH
#define ASTARGRID_HH

#include <QPair>

#include "types/vec2.hh"

/*
    Default das posições é em metros
    O campo é 9m x 6m
    Se fizermos uma grid de 9000 x 6000, cada célula terá 1mm de largura
    então, conversionFactor = 1000
    Se fizermos uma grid de 900 x 600, cada célula terá 10cm de largura
    então, conversionFactor = 100
    Se fizermos uma grid de 9 x 6, cada célula terá 1m de largura
    então, conversionFactor = 1
*/
template <int W, int H, int conversionFactor> class AStarGrid {
public:
    bool grid[W][H];

    AStarGrid() {
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                grid[i][j] = false;
            }
        }
    }

    void setObstacle(const Vec2 &pos, float radius) {
        double X = (pos.x() + (W / 2.0) / conversionFactor) * conversionFactor;
        double Y = (pos.y() + (H / 2.0) / conversionFactor) * conversionFactor;
        double R = radius * conversionFactor;
    
        int minX = static_cast<int>(std::floor(X - R));
        int maxX = static_cast<int>(std::ceil(X + R));
        int minY = static_cast<int>(std::floor(Y - R));
        int maxY = static_cast<int>(std::ceil(Y + R));
    
        minX = std::max(minX, 0);
        maxX = std::min(maxX, W - 1);
        minY = std::max(minY, 0);
        maxY = std::min(maxY, H - 1);
    
        for (int i = minX; i <= maxX; i++) {
            for (int j = minY; j <= maxY; j++) {
                grid[i][j] = true;
            }
        }
    }

    bool isObstacle(int x, int y) {
        return grid[x][y];
    }

    void clearGrid() {
        for (int i = 0; i < W; i++) {
            for (int j = 0; j < H; j++) {
                grid[i][j] = false;
            }
        }
    }

    const int width() const {
        return W;
    }

    const int height() const {
        return H;
    }

    static const QPair<int, int> convertVec2ToGrid(const Vec2 &pos) {
        int X = static_cast<int>((pos.x() + (W / 2.0) / conversionFactor) * conversionFactor);
        int Y = static_cast<int>((pos.y() + (H / 2.0) / conversionFactor) * conversionFactor);
        X = std::clamp(X, 0, W - 1);
        Y = std::clamp(Y, 0, H - 1);
        return QPair<int, int>(X, Y);
    }

    static const Vec2 convertGridToVec2(int x, int y) {
        return Vec2((x - W / 2.0) / conversionFactor, (y - H / 2.0) / conversionFactor);
    }
};

#endif