//
//  original.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef original_hpp
#define original_hpp

#include <string>
#include "sdf.hpp"

class Original : public SDF {
protected:
    Point Get( Grid &g, int x, int y ) override;
    void Put( Grid &g, int x, int y, const Point &p ) override;
    void Compare( Grid &g, Point &p, int x, int y, int offsetx, int offsety ) override;
    void GenerateSDF( Grid &g ) override;
public:
    void loadImage( int width, int height, unsigned char* image ) override;
    void run( int repeat, const std::string& name ) override;
    ~Original();
};

#endif /* original_hpp */
