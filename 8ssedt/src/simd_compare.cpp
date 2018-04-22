//
//  simd_compare.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//
#include <iostream>
#include "simd_compare.hpp"

inline void SimdCompare::GroupCompare(Grid &g, int x, int y, __m256i& offsets) {
    Point p = Get(g, x, y);
    
    /* Point other = Get( g, x+offsetx, y+offsety ); */
    int *offsetsPtr = (int *)&offsets;
    Point pn[4] = {
        Get(g, x + offsetsPtr[0], y + offsetsPtr[4]),
        Get(g, x + offsetsPtr[1], y + offsetsPtr[5]),
        Get(g, x + offsetsPtr[2], y + offsetsPtr[6]),
        Get(g, x + offsetsPtr[3], y + offsetsPtr[7]),
    };
    
    /* other.dx += offsetx; other.dy += offsety; */
    __m256i *pnPtr = (__m256i *)pn;
    // x0, y0, x1, y1, x2, y2, x3, y3 -> x0, x1, x2, x3, y0, y1, y2, y3
    static const __m256i mask = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
    __m256i vecCoords = _mm256_permutevar8x32_epi32(*pnPtr, mask);
    vecCoords = _mm256_add_epi32(vecCoords, offsets);
    
    /* other.DistSq() */
    int *coordsPtr = (int *)&vecCoords;
    // note that _mm256_mul_epi32 only applies on the lower 128 bits
    __m256i vecPermuted = _mm256_permute2x128_si256(vecCoords , vecCoords, 1);
    __m256i vecSqrDists = _mm256_add_epi64(_mm256_mul_epi32(vecCoords, vecCoords),
                                           _mm256_mul_epi32(vecPermuted, vecPermuted));
    
    /* if (other.DistSq() < p.DistSq()) p = other; */
    int64_t prevDist = p.DistSq(), index = -1;
    for (int i = 0; i < 4; ++i) {
        int64_t dist = *((int64_t *)&vecSqrDists + i);
        if (dist < prevDist) {
            prevDist = dist;
            index = i;
        }
    }
    if (index != -1) Put(g, x, y, { coordsPtr[index], coordsPtr[index + 4] });
}

void SimdCompare::GenerateSDF(Grid &g) {
    // Pass 0
    __m256i offsets0 = _mm256_setr_epi32(-1, -1, 0, 1, 0, -1, -1, -1);
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++)
            GroupCompare(g, x, y, offsets0);
        
        for (int x = imageWidth - 1; x >= 0; x--) {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, 1, 0 );
            Put( g, x, y, p );
        }
    }
    
    // Pass 1
    __m256i offsets1 = _mm256_setr_epi32(1, -1, 0, 1, 0, 1, 1, 1);
    for (int y = imageHeight - 1; y >= 0; y--) {
        for (int x = imageWidth - 1; x >= 0; x--)
            GroupCompare(g, x, y, offsets1);
        
        for (int x = 0; x < imageWidth; x++) {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, -1, 0 );
            Put( g, x, y, p );
        }
    }
}
