//
//  plain_original.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/23/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef plain_original_hpp
#define plain_original_hpp

#include <string>

class PlainOriginal {
    struct Point
    {
        int dx, dy;
        
        int DistSq() const { return dx*dx + dy*dy; }
    };
    
    struct Grid
    {
        Point *points;
    };
    
    int imageWidth, imageHeight;
    int gridWidth, gridHeight, numPoint;
    Grid grid1, grid2;
    Point inside = { 0, 0 };
    Point empty = { 9999, 9999 };
    Point Get( Grid &g, int x, int y );
    void Put( Grid &g, int x, int y, const Point &p );
    void Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety );
    void GenerateSDF( Grid &g );
public:
    void loadImage( int width, int height, unsigned char* image );
    void run( int repeat, const std::string& name );
    ~PlainOriginal();
};

#endif /* plain_original_hpp */
