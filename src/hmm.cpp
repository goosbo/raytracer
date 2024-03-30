#include "usefulStuff.h"
#include "hittableList.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
using namespace std;

int main() {
    camera cam;
    hittableList world;
    auto matGround = make_shared<lambertian>(color(0.08,0.08,0.08));
    world.add(make_shared<sphere>(point3(0,-1000,0),1000,matGround));
    for(int a = -11; a < 11; a++){
        for(int b = -11; b < 11; b++){
            point3 centre = point3(a + 0.9*randDouble(),0.2,b+0.9*randDouble());
            double matVal = randDouble();
            if ((centre - point3(4, 0.2, 0)).len() > 0.9) {
                if(matVal < 0.75){
                    auto matSphere = make_shared<lambertian>(color::random());
                    world.add(make_shared<sphere>(centre,0.2,matSphere));
                }

                else if(matVal < 0.9){
                    auto matSphere = make_shared<metal>(color::random(),randDouble(0.0,0.5));
                    world.add(make_shared<sphere>(centre,0.2,matSphere));
                }
                else{
                    auto matSphere = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(centre,0.2,matSphere));
                }
            }
            
        }
    } 
    auto mat1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, mat1));

    auto mat2 = make_shared<lambertian>(color(0.45, 0.35, 0.76));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, mat2));

    auto mat3 = make_shared<metal>(color(0.89, 0.76, 0.44), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, mat3));   
    cam.aspectRatio = 16.0/9.0;
    cam.imageWidth = 1200;
    cam.samplesPerPixel = 50;
    cam.maxDepth = 50;
    cam.vfov = 20;
    cam.lookFrom = point3(13,2,3);
    cam.lookAt = point3(0,0,0);
    cam.vUp = vec3(0,1,0);
    cam.defocusAngle = 0.6;
    cam.focusDist = 10;
    cam.render(world);
}