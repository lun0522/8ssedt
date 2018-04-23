//
//  simple_compare.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/23/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef simple_compare_hpp
#define simple_compare_hpp

#include "simd_compare.hpp"

class SimpleCompare : public SimdCompare {
protected:
    inline void SingleCompare(Grid &g, int x, int y, int offsetx, int offsety) {
        Point p = Get(g, x, y);
        Point other = Get(g, x + offsetx, y + offsety);
        other.dx += offsetx;
        other.dy += offsety;
        
        if (other.DistSq() < p.DistSq())
            Put(g, x, y, other);
    }
    void GenerateSDF(Grid &g) override;
};

#endif /* simple_compare_hpp */
