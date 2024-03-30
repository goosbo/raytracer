#pragma once
#include <iostream>
#include "vec3.h"
#include "usefulStuff.h"

using color = vec3;

double linearToGamma(double linear){
    return sqrt(linear);
}

void writeColor(std::ostream &out, color pixel, int samples){
    double r = pixel.x(), g = pixel.y(), b = pixel.z();
    auto scale = 1.0/samples;
    r*= scale; g *= scale; b *= scale;
    r = linearToGamma(r); g = linearToGamma(g); b = linearToGamma(b);
    interval intvl(0.000,0.999);

    out << (int)(intvl.clamp(r) *256) << ' '<< (int)(intvl.clamp(g)*256) << ' '<< (int)(intvl.clamp(b)*256)<<'\n';
}