//
//  main.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/20/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//
//  Modified from http://www.codersnotes.com/notes/signed-distance-fields/
//

#include <iostream>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

struct Point
{
    int dx, dy;
    
    int DistSq() const { return dx*dx + dy*dy; }
};

struct Grid
{
    Point *points;
};

int width, height;
Point inside = { 0, 0 };
Point empty = { 9999, 9999 };

namespace origin {
    Point Get( Grid &g, int x, int y )
    {
        // OPTIMIZATION: you can skip the edge check code if you make your grid
        // have a 1-pixel gutter.
        if ( x >= 0 && y >= 0 && x < width && y < height )
            return g.points[y*width+x];
        else
            return empty;
    }
    
    void Put( Grid &g, int x, int y, const Point &p )
    {
        g.points[y*width+x] = p;
    }
    
    void Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety )
    {
        Point other = Get( g, x+offsetx, y+offsety );
        other.dx += offsetx;
        other.dy += offsety;
        
        if (other.DistSq() < p.DistSq())
            p = other;
    }
    
    void GenerateSDF( Grid &g )
    {
        // Pass 0
        for (int y=0;y<height;y++)
        {
            for (int x=0;x<width;x++)
            {
                Point p = Get( g, x, y );
                Compare( g, p, x, y, -1,  0 );
                Compare( g, p, x, y,  0, -1 );
                Compare( g, p, x, y, -1, -1 );
                Compare( g, p, x, y,  1, -1 );
                Put( g, x, y, p );
            }
            
            for (int x=width-1;x>=0;x--)
            {
                Point p = Get( g, x, y );
                Compare( g, p, x, y, 1, 0 );
                Put( g, x, y, p );
            }
        }
        
        // Pass 1
        for (int y=height-1;y>=0;y--)
        {
            for (int x=width-1;x>=0;x--)
            {
                Point p = Get( g, x, y );
                Compare( g, p, x, y,  1,  0 );
                Compare( g, p, x, y,  0,  1 );
                Compare( g, p, x, y, -1,  1 );
                Compare( g, p, x, y,  1,  1 );
                Put( g, x, y, p );
            }
            
            for (int x=0;x<width;x++)
            {
                Point p = Get( g, x, y );
                Compare( g, p, x, y, -1, 0 );
                Put( g, x, y, p );
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    int channel;
    stbi_uc *testImage = stbi_load("test.jpg", &width, &height, &channel, 0);
    if (testImage == NULL) {
        std::cerr << "Failed to load image" << std::endl;
        return -1;
    }
    if (channel != 1) {
        std::cerr << "Input should be grayscale image" << std::endl;
        return -1;
    }
    
    std::cout << "Test image size: " << width << "x" << height << std::endl;
    size_t numPixel = width * height;
    auto createGrid = [numPixel] () -> Grid {
        Point *points = (Point *)malloc(numPixel * sizeof(Point));
        return { points };
    };
    auto copyGrid = [numPixel] (Grid& dst, Grid& src) {
        memcpy(dst.points, src.points, numPixel * sizeof(Point));
    };
    auto destroyGrid = [] (Grid& grid) {
        if (grid.points) free(grid.points);
    };
    
    Grid inGrid1 = createGrid(), inGrid2 = createGrid();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (testImage[y * width + x] < 128) {
                origin::Put(inGrid1, x, y, inside);
                origin::Put(inGrid2, x, y, empty);
            } else {
                origin::Put(inGrid2, x, y, inside);
                origin::Put(inGrid1, x, y, empty);
            }
        }
    }
    stbi_image_free(testImage);
    
    auto run = [&] (auto& func, const char* filename) -> double {
        Grid grid1 = createGrid(), grid2 = createGrid();
        copyGrid(grid1, inGrid1);
        copyGrid(grid2, inGrid2);
        
        auto start = std::chrono::system_clock::now();
        func(grid1);
        func(grid2);
        auto elapsed = std::chrono::system_clock::now() - start;
        
        stbi_uc *outputImage = (stbi_uc *)malloc(numPixel * sizeof(stbi_uc));
        for(int y = 0; y < height; ++y) {
            for (int x = 0 ; x < width; ++x) {
                // calculate the actual distance from the dx/dy
                int dist1 = (int)(sqrt((double)origin::Get(grid1, x, y).DistSq()));
                int dist2 = (int)(sqrt((double)origin::Get(grid2, x, y).DistSq()));
                int dist = dist1 - dist2;
                
                // clamp and scale for display purpose
                int c = dist * 3 + 128;
                if (c < 0) c = 0;
                if (c > 255) c = 255;
                
                outputImage[y * width + x] = c;
            }
        }
        stbi_write_jpg(filename, width, height, 1, outputImage, 100);
        
        destroyGrid(grid1);
        destroyGrid(grid2);
        free(outputImage);
        
        return elapsed.count();
    };
    
    std::cout << "Running original..." << std::endl;
    std::cout << run(origin::GenerateSDF, "original.jpg") << "s" << std::endl;
    
    destroyGrid(inGrid1);
    destroyGrid(inGrid2);
    
    return 0;
}
