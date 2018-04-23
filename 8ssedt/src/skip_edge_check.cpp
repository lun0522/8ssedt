//
//  skip_edge_check.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#include "skip_edge_check.hpp"

void SkipEdgeCheck::loadImage(int width, int height, unsigned char* image) {
    imageWidth = width;
    imageHeight = height;
    gridWidth = imageWidth + 2;
    gridHeight = imageHeight + 2;
    numPoint = gridWidth * gridHeight; // include padding
    grid1.points = (Point *)malloc(numPoint * sizeof(Point));
    grid2.points = (Point *)malloc(numPoint * sizeof(Point));
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            if (image[y * imageWidth + x] < 128) {
                Put(grid1, x, y, inside);
                Put(grid2, x, y, empty);
            } else {
                Put(grid2, x, y, inside);
                Put(grid1, x, y, empty);
            }
        }
    }
    for (int x = 0; x < imageWidth; ++x) { // top and buttom padding
        Put(grid2, x, -1, Get(grid2, x, 0));
        Put(grid1, x, -1, Get(grid1, x, 0));
        Put(grid2, x, imageHeight, Get(grid2, x, imageHeight - 1));
        Put(grid1, x, imageHeight, Get(grid1, x, imageHeight - 1));
    }
    for (int y = -1; y <= imageHeight; ++y) { // left and right padding
        Put(grid2, -1, y, Get(grid2, 0, y));
        Put(grid1, -1, y, Get(grid1, 0, y));
        Put(grid2, imageWidth, y, Get(grid2, imageWidth - 1, y));
        Put(grid1, imageWidth, y, Get(grid1, imageWidth - 1, y));
    }
}
