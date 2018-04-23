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
    inline Point SingleCompare(Grid &g, int x, int y, int offsetx, int offsety, Point other) {
        Point self = Get(g, x, y);
        other.dx += offsetx;
        other.dy += offsety;
        
        if (other.DistSq() < self.DistSq()) {
            Put(g, x, y, other);
            return other;
        } else {
            return self;
        }
    }
    void GenerateSDF(Grid &g) override;
};

#endif /* simple_compare_hpp */
