//
//  simple_compare.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/23/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#include "simple_compare.hpp"

void SimpleCompare::GenerateSDF(Grid &g) {
    // Pass 0
    static const __m256i offsets0 = _mm256_setr_epi32(-1, -1, 0, 1, 0, -1, -1, -1);
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x)
            GroupCompare(g, x, y, offsets0);
        
        for (int x = imageWidth - 1; x >= 0; --x)
            SingleCompare(g, x, y, 1, 0);
    }
    
    // Pass 1
    static const __m256i offsets1 = _mm256_setr_epi32(1, -1, 0, 1, 0, 1, 1, 1);
    for (int y = imageHeight - 1; y >= 0; --y) {
        for (int x = imageWidth - 1; x >= 0; --x)
            GroupCompare(g, x, y, offsets1);
        
        for (int x = 0; x < imageWidth; ++x)
            SingleCompare(g, x, y, -1, 0);
    }
}
