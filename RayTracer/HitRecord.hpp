#ifndef CS6400_HIT_RECORD_HPP
#define CS6400_HIT_RECORD_HPP

#include "Vector.hpp"
#include "Ray.hpp"
#include "Material.hpp"

#include <functional>
#include <memory>


struct HitRecord
{
	std::shared_ptr<Material> material;
	Vector n;
	Vector at;
	Ray r;
    
	Vector solid;
    
    double u;
    double v;
    
	Color getColor();
	HitRecord():material(std::make_shared<NullMaterial>()),n(0,0,0),at(0,0,0),r(){}
	HitRecord(std::shared_ptr<Material> m,Vector n,Vector at,Ray r):material(m),n(n),at(at),r(r){}
};


#endif