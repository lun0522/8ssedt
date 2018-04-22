//
//  simd_compare.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/21/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//

#include <immintrin.h>
#include "simd_compare.hpp"

void SimdCompare::GenerateSDF(Grid &g) {
    // Pass 0
    int offsets[8] = {
        -1,  0, -1,  1,
         0, -1, -1, -1,
    };
    __m256i vecOffsets = _mm256_load_si256((__m256i *)offsets);
    
    for (int y = 0; y < imageHeight; y++) {
        for (int x = 0; x < imageWidth; x++) {
            Point p = Get(g, x, y);
            Point pn[4] = {
                Get(g, x - 1, y    ),
                Get(g, x - 1, y - 1),
                Get(g, x,     y - 1),
                Get(g, x + 1, y - 1),
            };
            int coords[8] = {
                pn[0].dx, pn[1].dx, pn[2].dx, pn[3].dx,
                pn[0].dy, pn[1].dy, pn[2].dy, pn[3].dy,
            };
            __m256i vecCoords = _mm256_load_si256((__m256i *)coords);
            __m256i vecBiased = _mm256_add_epi32(vecCoords, vecOffsets);
            __m256i vecSqrBiased = _mm256_mul_epi32(vecBiased, vecBiased);
            __m128i *vecX = (__m128i *)(&vecSqrBiased);
            __m128i *vecY = (__m128i *)((int *)(&vecSqrBiased) + 4);
            __m128i vecSqrDists = _mm_add_epi32(*vecX, *vecY);
            int prevDist = p.DistSq(), index = -1;
            for (int i = 0; i < 4; ++i) {
                int dist = *((int *)(&vecSqrDists) + i);
                if (dist < prevDist) {
                    prevDist = dist;
                    index = i;
                }
            }
            if (index != -1) {
                pn[index].dx += *((int *)(&vecBiased) + index);
                pn[index].dy += *((int *)(&vecBiased) + index + 4);
                Put(g, x, y, pn[index]);
            }
        }
        
        for (int x = imageWidth - 1; x >= 0; x--) {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, 1, 0 );
            Put( g, x, y, p );
        }
    }
    
    // Pass 1
    for (int y = imageHeight - 1; y >= 0; y--) {
        for (int x = imageWidth - 1; x >= 0; x--) {
            Point p = Get( g, x, y );
            Compare( g, p, x, y,  1,  0 );
            Compare( g, p, x, y,  0,  1 );
            Compare( g, p, x, y, -1,  1 );
            Compare( g, p, x, y,  1,  1 );
            Put( g, x, y, p );
        }
        
        for (int x = 0; x < imageWidth; x++) {
            Point p = Get( g, x, y );
            Compare( g, p, x, y, -1, 0 );
            Put( g, x, y, p );
        }
    }
}
