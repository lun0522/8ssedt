//
//  sdf.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef sdf_hpp
#define sdf_hpp

#include <string>

struct Point
{
    int dx, dy;
    
    int DistSq() const { return dx*dx + dy*dy; }
};

struct Grid
{
    Point *points;
};

class SDF {
protected:
    int imageWidth, imageHeight;
    int gridWidth, gridHeight, numPoint;
    Grid grid1, grid2;
    Point inside = { 0, 0 };
    Point empty = { 9999, 9999 };
    virtual Point Get( Grid &g, int x, int y ) = 0;
    virtual void Put( Grid &g, int x, int y, const Point &p ) = 0;
    virtual void Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety ) = 0;
    virtual void GenerateSDF( Grid &g ) = 0;
public:
    virtual void loadImage( int width, int height, unsigned char* image ) = 0;
    virtual void run( int repeat, const std::string& name ) = 0;
};

#endif /* sdf_hpp */
