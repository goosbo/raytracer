#pragma once
#include "usefulStuff.h"
const double infinity = std::numeric_limits<double>::infinity();
class interval{
    public:
        double min, max;

        interval(): min(infinity), max(-infinity) {}
        interval(double Min,double Max): min(Min), max(Max) {}

        bool contains(double x){
            return ( min <= x && max >= x);
        }

        bool surrounds(double x){
            return (min < x && max > x);
        }

        double clamp(double x) {
            if( x < min) return min;
            if( x > max) return max;
            return x;
        }

        const static interval empty, universe;
};

const static interval empty(infinity, -infinity);
const static interval universe(-infinity, infinity);