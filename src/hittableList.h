#pragma once
#include <memory>
#include <vector>
#include "hittable.h"

class hittableList: public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects;

        hittableList() {}
        hittableList(std::shared_ptr<hittable> object){ add(object);}

        void add(std::shared_ptr<hittable> object){
            objects.push_back(object);
        }

        void clear() {objects.clear();}

        bool hit(const ray &r, interval ray_t, hitRecord &rec) const override {
            hitRecord tempRec;
            double closest = ray_t.max;
            bool hitAnything = false;
            
            for(const auto &object : objects){
                if(object->hit(r, interval(ray_t.min,closest), tempRec)){
                    hitAnything = true;
                    closest = tempRec.t;
                    rec = tempRec;
                }
            }
            return hitAnything;
        }
};