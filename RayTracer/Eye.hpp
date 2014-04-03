#ifndef EYE_HPP
#define EYE_HPP

#include "Preprocess.hpp"
#include "Vector.hpp"
#include "Keyframed.hpp"
#include "Ray.hpp"
#include <random>
#include <cmath>

class Eye
{
    Keyframed<Vector> position, lookAt, up;
    Keyframed<double> planeDist, planeWidth, planeHeight;
    double lensRadius;
public:
    int resolutionX, resolutionY;
    Eye():position(),lookAt(),up(),planeDist(),planeWidth(),planeHeight(),lensRadius(),resolutionX(),resolutionY(){}
    Eye(Keyframed<Vector> p, Keyframed<Vector> l, Keyframed<Vector> u, Keyframed<double> d, Keyframed<double> w, Keyframed<double> h, double r, int x, int y):position(p),lookAt(l),up(u),planeDist(d),planeWidth(w),planeHeight(h),lensRadius(r),resolutionX(x),resolutionY(y){}
    Ray makeRay(double time, int x, int y);
};

#endif