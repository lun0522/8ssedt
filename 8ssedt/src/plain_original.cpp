//
//  plain_original.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/23/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#include <math.h>
#include <iostream>
#include <chrono>
#include "stb_image.h"
#include "stb_image_write.h"
#include "plain_original.hpp"

PlainOriginal::Point PlainOriginal::Get( Grid &g, int x, int y )
{
    // OPTIMIZATION: you can skip the edge check code if you make your grid
    // have a 1-pixel gutter.
    if ( x >= 0 && y >= 0 && x < imageWidth && y < imageHeight )
        return g.points[y*imageWidth+x];
    else
        return empty;
}

void PlainOriginal::Put( Grid &g, int x, int y, const Point &p )
{
    g.points[y*imageWidth+x] = p;
}

void PlainOriginal::Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety )
{
    Point other = Get( g, x+offsetx, y+offsety );
    other.dx += offsetx;
    other.dy += offsety;
    
    if (other.DistSq() < p.DistSq())
        p = other;
}

void PlainOriginal::GenerateSDF( Grid &g )
{
    // Pass 0
    for (int y=0;y<imageHeight;y++)
    {
        for (int x=0;x<imageWidth;x++)
        {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, -1,  0 );
            Compare( g, p, x, y,  0, -1 );
            Compare( g, p, x, y, -1, -1 );
            Compare( g, p, x, y,  1, -1 );
            Put( g, x, y, p );
        }
        
        for (int x=imageWidth-1;x>=0;x--)
        {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, 1, 0 );
            Put( g, x, y, p );
        }
    }
    
    // Pass 1
    for (int y=imageHeight-1;y>=0;y--)
    {
        for (int x=imageWidth-1;x>=0;x--)
        {
            Point p = Get( g, x, y );
            Compare( g, p, x, y,  1,  0 );
            Compare( g, p, x, y,  0,  1 );
            Compare( g, p, x, y, -1,  1 );
            Compare( g, p, x, y,  1,  1 );
            Put( g, x, y, p );
        }
        
        for (int x=0;x<imageWidth;x++)
        {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, -1, 0 );
            Put( g, x, y, p );
        }
    }
}

void PlainOriginal::loadImage( int width, int height, unsigned char* image ) {
    imageWidth = gridWidth = width;
    imageHeight = gridHeight = height;
    numPoint = gridWidth * gridHeight;
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
}

void PlainOriginal::run( int repeat, const std::string& name ) {
    std::cout << "Running " << name << "..." << std::endl;
    double elaspsedTime = 0.0;
    Grid testGrid1, testGrid2;
    testGrid1.points = (Point *)malloc(numPoint * sizeof(Point));
    testGrid2.points = (Point *)malloc(numPoint * sizeof(Point));
    for (int i = 0; i < repeat; ++i) {
        memcpy(testGrid1.points, grid1.points, numPoint * sizeof(Point));
        memcpy(testGrid2.points, grid2.points, numPoint * sizeof(Point));
        
        auto start = std::chrono::system_clock::now();
        GenerateSDF(testGrid1);
        GenerateSDF(testGrid2);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::ratio<1>> elapsed = end - start;
        elaspsedTime += elapsed.count();
    }
    std::cout << "Finished after " << elaspsedTime / repeat << "s on average" << std::endl;
    
    stbi_uc *output = (stbi_uc *)malloc(imageWidth * imageHeight * sizeof(stbi_uc));
    for(int y = 0; y < imageHeight; ++y) {
        for (int x = 0 ; x < imageWidth; ++x) {
            // calculate the actual distance from the dx/dy
            int dist1 = (int)(sqrt((double)Get(testGrid1, x, y).DistSq()));
            int dist2 = (int)(sqrt((double)Get(testGrid2, x, y).DistSq()));
            int dist = dist1 - dist2;
            
            // clamp and scale for display purpose
            int c = dist * 3 + 128;
            if (c < 0) c = 0;
            if (c > 255) c = 255;
            
            output[y * imageWidth + x] = c;
        }
    }
    stbi_write_jpg((name + ".jpg").c_str(), imageWidth, imageHeight, 1, output, 100);
    
    if (testGrid1.points) free(testGrid1.points);
    if (testGrid2.points) free(testGrid2.points);
}

PlainOriginal::~PlainOriginal() {
    if (grid1.points) free(grid1.points);
    if (grid2.points) free(grid2.points);
    std::cout << "Grids freed" << std::endl;
}
