//
//  main.cpp
//  8ssedt
//
//  Created by Pujun Lun on 4/20/18.
//  Copyright © 2018 Pujun Lun. All rights reserved.
//
//  Modified from http://www.codersnotes.com/notes/signed-distance-fields/
//  May need to specify a flag for: Build Settings -> Code Generation -> Optimization Level -> Debug
//  Also set Build Settings -> Code Generation -> Enable Additional Vector Extensions to AVX 2

#include <iostream>
#include <string>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "original.hpp"
#include "skip_edge_check.hpp"
#include "simd_compare.hpp"
#include "simple_compare.hpp"
#include "plain_original.hpp"
#include "all_together.hpp"

int width, height;
stbi_uc *image;

template <typename T>
void run(const std::string& name) {
    T sdf;
    sdf.loadImage(width, height, image);
    sdf.run(1, name);
}

int main(int argc, const char * argv[]) {
    int channel;
    image = stbi_load("test.jpg", &width, &height, &channel, 0);
    if (image == NULL) {
        std::cerr << "Failed to load image" << std::endl;
        return -1;
    }
    if (channel != 1) {
        std::cerr << "Input should be grayscale image" << std::endl;
        return -1;
    }
    std::cout << "Test image size: " << width << "x" << height << std::endl;
    
    run<Original>("original");
    run<SkipEdgeCheck>("skip_edge_check");
    run<SimdCompare>("simd_compare");
    run<SimpleCompare>("simple_compare");
    
    run<PlainOriginal>("plain_original");
    run<AllTogether>("all_together");
    
    stbi_image_free(image);
    return 0;
}
