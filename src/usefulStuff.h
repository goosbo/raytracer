#pragma once


#include <cstdlib>



const double pi = 3.1415926535897932385;

inline double degToRad(double degree){
    return pi * degree / 180.0;
}

inline double randDouble(){
    return rand()/(RAND_MAX+1.0);
}

inline double randDouble(double min, double max){
    return min + (max-min) * randDouble();
}
#include <cmath>
#include <limits>
#include <memory>
#include "ray.h"
#include "interval.h"
#include "color.h"
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

