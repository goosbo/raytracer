#pragma once
#include "usefulStuff.h"
#include <iostream>



class vec3 {

    public:
        double v[3];
        vec3(): v{0,0,0} {}
        vec3(double x, double y, double z): v{x,y,z} {}
        double x() const {return v[0];}
        double y() const {return v[1];}
        double z() const {return v[2];}

        vec3 operator-() const{return vec3(-v[0],-v[1],-v[2]);}
        double operator[](int i) const{return v[i];}
        double& operator[](int i) {return v[i];}

        vec3& operator+=(const vec3 &o){
            for(int i = 0; i < 3; i++) v[i] += o[i];
            return *this;
        }
        
        vec3& operator-=(const vec3 &o){
            return *this += -o;
        }

        vec3& operator*=(double m){
            for(int i = 0; i < 3; i++) v[i] *= m;
            return *this;
        }

        vec3& operator/=(double d){
            return *this *= 1/d;
        }

        double lensq(){
            return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
        }

        double len(){
            return std::sqrt(lensq());
        }

        static vec3 random(){
            return vec3(randDouble(),randDouble(),randDouble());
        }

        static vec3 random(double min,double max){
            return vec3(randDouble(min, max),randDouble(min, max),randDouble(min, max));
        }

        bool nearZero(){
            auto s = 1e-8;
            return (fabs(v[0]) < s && fabs(v[1])< s && fabs(v[2]) <s);
        }
};

using point3 = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v){
    return out<<v[0]<<' '<<v[1]<<' '<<v[2];
}
inline vec3 operator+(const vec3 &u,const vec3 &v){
    return vec3(u[0]+v[0],u[1]+v[1],u[2]+v[2]);
}
inline vec3 operator-(const vec3 &u,const vec3 &v){
    return vec3(u[0]-v[0],u[1]-v[1],u[2]-v[2]);
}
inline vec3 operator*(const vec3 &u,const vec3 &v){
    return vec3(u[0]*v[0],u[1]*v[1],u[2]*v[2]);
}
inline vec3 operator*(const vec3 &v,const double m){
    return vec3(m*v[0],m*v[1],m*v[2]);
}
inline vec3 operator*(const double m,const vec3 &v){
    return v*m;
}
inline vec3 operator/(const vec3 &v,const double d){
    return v*(1/d);
}
inline double dot(const vec3 &u, const vec3 &v){
    return u[0]*v[0]+u[1]*v[1]+u[2]*v[2];
}
inline vec3 cross(const vec3 &u, const vec3 &v){
    return vec3(u[1]*v[2]-u[2]*v[1],u[2]*v[0]-u[0]*v[2],u[0]*v[1]-u[1]*v[0]);
}
inline vec3 unitVec(vec3 u){
    return u/u.len();
}

inline vec3 randomInUnitSphere(){
    while(true){
        auto p = vec3::random(-1,1);
        if(p.lensq()< 1) return p;
    }
}
inline vec3 randomUnitVector(){
    return unitVec(randomInUnitSphere());
}
inline vec3 randomInHemisphere(const vec3 &normal) {
    vec3 v = randomUnitVector();
    if(dot(normal,v)>0.0) return v;
    else return -v;
}
inline vec3 randInUnitDisk() {
    while (true) {
        auto p = vec3(randDouble(-1,1), randDouble(-1,1), 0);
        if (p.lensq() < 1)
            return p;
    }
}
vec3 reflect(const vec3 &v, const vec3 &n){
    return v - 2*dot(v,n)*n;
}
vec3 refract(const vec3 &uv, const vec3 &n, double rIndex){
    double cos_theta = fmin(dot(-uv,n),1.0);
    auto r_perp = rIndex * (uv + cos_theta*n);
    auto r_parallel = -sqrt(fabs(1.0 - r_perp.lensq())) * n;
    return r_perp + r_parallel;
}