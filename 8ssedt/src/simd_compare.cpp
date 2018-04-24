//
//  simd_compare.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#include "simd_compare.hpp"

void SimdCompare::GenerateSDF(Grid &g) {
    // Pass 0
    static const __m256i offsets0 = _mm256_setr_epi32(-1, -1, 0, 1, 0, -1, -1, -1);
    for (int y = 0; y < imageHeight; ++y) {
        Point prev = Get(g, -1, y);
        for (int x = 0; x < imageWidth; ++x)
            prev = GroupCompare(g, prev, x, y, offsets0);
        
        for (int x = imageWidth - 1; x >= 0; --x) {
            Point p = Get(g, x, y);
            Compare(g, p, x, y, 1, 0);
            Put(g, x, y, p);
        }
    }
    
    // Pass 1
    static const __m256i offsets1 = _mm256_setr_epi32(1, -1, 0, 1, 0, 1, 1, 1);
    for (int y = imageHeight - 1; y >= 0; --y) {
        Point prev = Get(g, imageWidth, y);
        for (int x = imageWidth - 1; x >= 0; --x)
            prev = GroupCompare(g, prev, x, y, offsets1);
        
        for (int x = 0; x < imageWidth; ++x) {
            Point p = Get(g, x, y);
            Compare(g, p, x, y, -1, 0);
            Put(g, x, y, p);
        }
    }
}
