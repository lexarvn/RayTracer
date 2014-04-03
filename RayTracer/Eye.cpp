#include "Eye.hpp"

#ifdef ADD_MOTION_BLUR
namespace
{
    double fallingCosine( double xMin, double xMax )
    {
        static std::default_random_engine g;
        static std::uniform_real_distribution<double> d(0,1);
        double b = 2*( xMax - xMin); // scale parameter
        return xMin + b*(asin(d(g))/M_PI);
    }
}
#endif

Ray Eye::makeRay(double time, int x, int y)
{
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> uni(-1,1);
    
    auto p = position(time);
    
    Vector w = lookAt(time)-p;
    w.normalize();
    Vector u = w.cross(up(time));
    u.normalize();
    Vector v = u.cross(w);
    v.normalize();
    
    u *= planeWidth(time)/resolutionX;
    v *= planeHeight(time)/resolutionY;
    
    auto pixelLocation = p + planeDist(time)*w + (x-resolutionX/2)*u + (y-resolutionY/2)*v;
    
#ifdef ADD_ANTI_ALIAS
    pixelLocation += u*uni(generator) + v*uni(generator);
#endif
    
    Ray r;
    
    r.time = time;
#ifdef ADD_MOTION_BLUR
    r.time -= static_cast<int>(fallingCosine(0,9))/20.0;
    if(r.time < 0)
        r.time = 0;
#endif
    
    r.origin = p;
#ifdef ADD_DEPTH_OF_FIELD
    r.origin += lensRadius*u*uni(generator) + lensRadius*v*uni(generator);
#endif
    
    r.direction = pixelLocation - r.origin;
    r.direction.normalize();
    
    r.depth = 0;
    
    return r;
}

