#pragma once
#include "usefulStuff.h"

class hitRecord;

class material {
    public:
    virtual ~material() = default;

    virtual bool scatter(const ray &rayIn, const hitRecord &rec, color &attenuation, ray &scattered) const = 0;
};

class lambertian: public material {
    public:
    lambertian(const color &a): albedo(a) {}
    bool scatter(const ray &rayIn, const hitRecord &rec, color &attenuation, ray &scattered) const override {
        auto scatterDirection = rec.normal + randomUnitVector();
        if(scatterDirection.nearZero()) scatterDirection = rec.normal;
        scattered = ray(rec.point,scatterDirection);
        attenuation = albedo;
        return true;
    }
    private:
    color albedo;
};

class metal: public material {
    public:
    metal(const color &a,double f): albedo(a),fuzz(f<1?f:1) {}
    bool scatter(const ray &rayIn, const hitRecord &rec, color &attenuation, ray &scattered) const override {
        auto reflectedDirection = reflect(unitVec(rayIn.direction()),rec.normal);
        scattered = ray(rec.point,reflectedDirection + fuzz*randomUnitVector());
        attenuation = albedo;
        return (dot(rec.normal,scattered.direction())>0);
    }
    private:
    color albedo;
    double fuzz;
};

class dielectric: public material {
    public:
    dielectric(double rIndex): refIndex(rIndex) {};
    bool scatter(const ray &rayIn, const hitRecord &rec, color &attenuation, ray &scattered) const override {
        attenuation = color(1,1,1);
        double refractiveIndex = rec.frontFace? 1.0/refIndex:refIndex;
        auto rayUnit = unitVec(rayIn.direction());
        auto cos_theta = dot(-rayUnit,rec.normal);
        auto sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        bool cannotRefract = refractiveIndex * sin_theta > 1.0;
        vec3 direction;
        if(cannotRefract || reflectance(cos_theta,refractiveIndex) > randDouble()){
            direction = reflect(rayUnit,rec.normal);
        }
        else direction = refract(rayUnit,rec.normal,refractiveIndex);
        scattered = ray(rec.point, direction);
        return true;
    }
    private:
    double refIndex;
    static double reflectance(double cos_theta,double refracIndex){
        auto r0 = (1-refracIndex)/(1+refracIndex);
        r0 = r0*r0;
        return r0 + (1-r0)*pow(1-cos_theta,5);
    }
};