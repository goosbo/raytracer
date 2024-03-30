#pragma once
#include "usefulStuff.h"
#include "hittableList.h"
#include "color.h"
#include <fstream>
#include <iostream>
#include "material.h"
using namespace std;
class camera {
    public:

        ofstream file;
        int imageWidth = 100;
        double aspectRatio = 1;
        int samplesPerPixel = 10;
        int maxDepth = 10;
        double vfov = 90;
        point3 lookFrom = point3(0,0,-1);
        point3 lookAt= point3(0,0,0);
        vec3 vUp= vec3(0,1,0);
        double defocusAngle = 0;
        double focusDist = 10;

        void render(const hittableList &world){
            file.open ("out.txt", ios::out | ios::trunc);
            initialize();
            file << "P3\n" << imageWidth << " " << imageHeight <<"\n255\n";
            for(int j = 0; j < imageHeight; j++) {
                clog << "Scan lines remaining: "<< (imageHeight - j) << ' ' << std::flush;
                for(int i = 0; i < imageWidth; i++) {
                    color pixel(0,0,0);
                    for(int s = 0; s < samplesPerPixel; s++){
                        ray r = getRay(i,j);
                        pixel += rayColor(r, maxDepth,world);
                    }
                    writeColor(file,pixel,samplesPerPixel);
                }
            }
            file.close();
            clog << "Done\n";
        }
    
    private:
        
        int imageHeight;
        
        point3 cameraCentre, pixel00_loc;
        vec3 pixel_delta_u, pixel_delta_v; 
        vec3 u,v,w;
        vec3 defocusDiscU, defocusDiscV;

        void initialize(){
            imageHeight = (int)(imageWidth/aspectRatio);
            imageHeight = imageHeight >1? imageHeight: 1;
            cameraCentre = lookFrom;
            double theta = degToRad(vfov);
            auto h = tan(theta/2);
            double viewportHeight = 2.0 * h * focusDist;
            double viewportWidth = viewportHeight * (double)imageWidth/imageHeight;
            
            w = unitVec(lookFrom - lookAt);
            u = unitVec(cross(vUp,w));
            v = cross(w,u);
            
            auto viewport_u = viewportWidth*u;
            auto viewport_v = -viewportHeight*v;
            pixel_delta_u = viewport_u/imageWidth;
            pixel_delta_v = viewport_v/imageHeight;

            auto viewportUpperLeft = cameraCentre - focusDist*w - viewport_u/2 -viewport_v/2;
            pixel00_loc = viewportUpperLeft + pixel_delta_u/2 + pixel_delta_v/2;

            auto defocusDiscRadius = focusDist * tan(degToRad(defocusAngle/2));
            defocusDiscU = defocusDiscRadius * u;
            defocusDiscV = defocusDiscRadius * v;

        }

        color rayColor(const ray &r, int depth, const hittableList &world){
            hitRecord rec;
            if(depth <= 0) return color(0,0,0);
            if(world.hit(r,interval(0.001,infinity),rec)){
                ray scattered; color attenuation;
                if(rec.mat->scatter(r,rec,attenuation,scattered)){
                    return attenuation*rayColor(scattered,depth-1, world);
                }
                return color(0,0,0);
            }

            auto rUnit = unitVec(r.direction());
            auto a = 0.5*(rUnit.y()+1.0);
            return (1-a)*color(1,1,1) + a*color(0.72,0.82,0.92);
    
        }    

        ray getRay(int i, int j) const{
            auto pixelCentre = pixel00_loc + i*pixel_delta_u + j*pixel_delta_v;
            auto pixelSample = pixelCentre + pixelSampleSquare();
            auto rayOrigin = defocusAngle<= 0? cameraCentre: defocusDiskSample();
            auto rayDir = pixelSample - rayOrigin;
            return ray(rayOrigin,rayDir);
        }

        color pixelSampleSquare() const{
            return (randDouble() - 0.5) * pixel_delta_u + (randDouble() - 0.5) * pixel_delta_v;
        }

        point3 defocusDiskSample() const{
            auto p = randInUnitDisk();
            return cameraCentre + p[0] * defocusDiscU + p[1] * defocusDiscV;
        }
};