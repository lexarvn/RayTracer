#include "PerlinNoise.hpp"

namespace  {
    
    double interpolate(double a,double b,double x)
    {
        auto ft=x * M_PI;
        auto f=(1.0-cos(ft))* 0.5;
        return a*(1.0-f)+b*f;
    }

    double findNoise(Vector v)
    {
        static std::map<Vector, double> memo;
        auto it = memo.find(v);
        if(it != memo.end())
            return it->second;
        
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(-1.0,1.0);
        
        auto val = distribution(generator);
        
        memo.insert(std::make_pair(v, val));
        return val;
    }

    double smoothNoise(Vector v)
    {
        static std::map<Vector, double> memo;
        auto it = memo.find(v);
        if(it != memo.end())
            return it->second;
        
        auto corners =   (findNoise(Vector(v.x-1, v.y-1, v.z-1))
                          + findNoise(Vector(v.x+1, v.y-1, v.z-1))
                          + findNoise(Vector(v.x-1, v.y+1, v.z-1))
                          + findNoise(Vector(v.x+1, v.y+1, v.z-1))
                          + findNoise(Vector(v.x-1, v.y-1, v.z+1))
                          + findNoise(Vector(v.x+1, v.y-1, v.z+1))
                          + findNoise(Vector(v.x-1, v.y+1, v.z+1))
                          + findNoise(Vector(v.x+1, v.y+1, v.z+1))) / 64.0;
        auto sideCorners =   (findNoise(Vector(v.x-1, v.y-1, v.z))
                              + findNoise(Vector(v.x+1, v.y-1, v.z))
                              + findNoise(Vector(v.x-1, v.y+1, v.z))
                              + findNoise(Vector(v.x+1, v.y+1, v.z))
                              + findNoise(Vector(v.x,   v.y-1, v.z-1))
                              + findNoise(Vector(v.x,   v.y+1, v.z-1))
                              + findNoise(Vector(v.x,   v.y-1, v.z+1))
                              + findNoise(Vector(v.x,   v.y+1, v.z+1))) / 32.0;
        auto sides = (findNoise(Vector(v.x-1, v.y,   v.z))
                      + findNoise(Vector(v.x+1, v.y,   v.z))
                      + findNoise(Vector(v.x,   v.y-1, v.z))
                      + findNoise(Vector(v.x,   v.y+1, v.z))
                      + findNoise(Vector(v.x,   v.y,   v.z-1))
                      + findNoise(Vector(v.x,   v.y,   v.z+1)))/12.0;
        auto center = findNoise(v)/8.0;
        
        auto val = corners + sideCorners + sides + center;
        
        memo.insert(std::make_pair(v, val));
        return val;
    }

    double interpolateNoise(Vector v)
    {
        
//        static std::map<Vector, double> memo;
//        auto it = memo.find(v);
//        if(it != memo.end())
//            return it->second;
        
        Vector floor;
        floor.x = (int)v.x;
        floor.y = (int)v.y;
        floor.z = (int)v.z;
        
        auto fraction = v-floor;
        
        auto p111 = smoothNoise(Vector(floor.x,  floor.y,  floor.z  ));
        auto p211 = smoothNoise(Vector(floor.x+1,floor.y,  floor.z  ));
        auto p121 = smoothNoise(Vector(floor.x,  floor.y+1,floor.z  ));
        auto p221 = smoothNoise(Vector(floor.x+1,floor.y+1,floor.z  ));
        auto p112 = smoothNoise(Vector(floor.x,  floor.y,  floor.z+1));
        auto p212 = smoothNoise(Vector(floor.x+1,floor.y,  floor.z+1));
        auto p122 = smoothNoise(Vector(floor.x,  floor.y+1,floor.z+1));
        auto p222 = smoothNoise(Vector(floor.x+1,floor.y+1,floor.z+1));
        
        auto ix11 = interpolate(p111, p211, fraction.x);
        auto ix21 = interpolate(p121, p221, fraction.x);
        auto i1y1 = interpolate(ix11, ix21, fraction.y);
        
        auto ix12 = interpolate(p112, p212, fraction.x);
        auto ix22 = interpolate(p122, p222, fraction.x);
        auto i1y2 = interpolate(ix12, ix22, fraction.y);
        
        auto val = interpolate(i1y1, i1y2, fraction.z);
        
//        memo.insert(std::make_pair(v, val));
        return val;
    }
}


double PerlinNoise::operator()(Vector v)
{
    if(v.x < 0)
        v.x =-v.x;
    if(v.y < 0)
        v.y =-v.y;
    if(v.z < 0)
        v.z =-v.z;
    
    v += Vector(1000);
    
    auto it = memo.find(v);
    if(it != memo.end())
        return it->second;
    
    double total = 0;
    
    for(unsigned int i=0; i<octaves; ++i)
    {
        auto frequency = pow(2, i);
        auto amplitude = pow(persistence, i);
        
        total += interpolateNoise(v*frequency) * amplitude;
        
    }
    
    return total;
    
    memo.insert(std::make_pair(v, total));
    return total;
    
}
