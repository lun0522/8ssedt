//
//  all_together.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/23/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef all_together_hpp
#define all_together_hpp

#include <string>
#include <immintrin.h>

class AllTogether {
    struct Point {
        int dx, dy;
        int distSq() const { return dx * dx + dy * dy; }
    };
    struct Grid {
        Point *points;
    };
    int imageWidth, imageHeight;
    int gridWidth, gridHeight, numPoint;
    Grid grid1, grid2;
    Point inside = { 0, 0 };
    Point empty = { 16384, 16384 };
    Point get(Grid &g, int x, int y);
    void put(Grid &g, int x, int y, const Point &p);
    Point groupCompare(Grid &g, Point other, int x, int y, const __m256i& offsets);
    Point singleCompare(Grid &g, Point other, int x, int y, int offsetx, int offsety);
    void generateSDF(Grid &g);
public:
    void loadImage(int width, int height, unsigned char* image);
    void run(int repeat, const std::string& name);
    ~AllTogether();
};

#endif /* all_together_hpp */
