//
//  simd_compare.hpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#ifndef simd_compare_hpp
#define simd_compare_hpp

#include "skip_edge_check.hpp"

class SimdCompare : public SkipEdgeCheck {
protected:
    void GenerateSDF(Grid &g) override;
};

#endif /* simd_compare_hpp */
