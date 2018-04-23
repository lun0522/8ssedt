//
//  skip_edge_check.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef skip_edge_check_hpp
#define skip_edge_check_hpp

#include "original.hpp"

class SkipEdgeCheck : public Original {
protected:
    Point empty = { 16384, 16384 };
    inline Point Get(Grid &g, int x, int y) override {
        return g.points[(y + 1) * gridWidth + (x + 1)];
    }
    inline void Put(Grid &g, int x, int y, const Point &p) override {
        g.points[(y + 1) * gridWidth + (x + 1)] = p;
    }
public:
    void loadImage(int width, int height, unsigned char* image) override;
};

#endif /* skip_edge_check_hpp */
