#pragma once
#include "hittable.h"

class sphere: public hittable{
    private:
        point3 centre;
        double radius;
        shared_ptr<material> mat;
    public:
        sphere(point3 cntre, double rd,shared_ptr<material> material_): centre(cntre), radius(rd),mat(material_) {}
        bool hit(const ray &r, interval ray_t, hitRecord &rec) const override {
            auto oc = r.origin() - centre;    
            auto a = r.direction().lensq();
            auto h =  dot(r.direction(),oc);
            auto c = oc.lensq() - radius*radius;
            double discriminant = h*h-a*c; 
            if(discriminant <=0) return false;
            double root = (-h - std::sqrt(discriminant))/a;
            if(!ray_t.surrounds(root)){
                root = (-h +std::sqrt(discriminant))/a;
                if(!ray_t.surrounds(root)) return false;
            }
            rec.t = root;
            rec.point = r.at(root);
            rec.setFaceNormal(r,(rec.point - centre)/radius);
            rec.mat = mat;
            return true;
        }
};