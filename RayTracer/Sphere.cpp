#include "Sphere.hpp"

Sphere::Sphere(Keyframed<Vector> center, Keyframed<double> radius, std::shared_ptr<Material> m)
:Shape(m,BoundingBox(getKeyframedX(center)-radius,getKeyframedX(center)+radius
                    ,getKeyframedY(center)-radius,getKeyframedY(center)+radius
                    ,getKeyframedZ(center)-radius,getKeyframedZ(center)+radius))
        ,center(center)
        ,radius(radius){}

Vector Sphere::getCenter(double t)
{
    return center(t);
}

Vector Sphere::getRandomPoint(Vector n, double t)
{
    auto c = center(t);
    auto r = radius(t);
    
    static std::default_random_engine g;
    static std::uniform_real_distribution<double> d(-1,1);
    
    auto theta = d(g)*M_PI_2;
    auto phi = (d(g)+1)*M_PI;
    
    auto p = Vector(r*cos(theta)*cos(phi), r*cos(theta)*sin(phi), r*sin(theta));
    
    if(n.dot(p)>1)
        p = -p;
    p *= 1.001;
    return p+c;
}

boost::optional<double> Sphere::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    auto p = r.origin - center(r.time);
    auto rad = radius(r.time);
    
    auto c = p.dot(p) - pow(rad,2);
    auto b = r.direction.dot(p)*2;
    auto a = r.direction.dot(r.direction);
    auto dsc = b*b - 4*a*c;
    if(dsc <= 0) return boost::optional<double>();
    auto result = (-b - std::sqrt(dsc)) / (2*a);
    if(result > 0) return boost::optional<double>(result);
    result = (-b + std::sqrt(dsc)) / (2*a);
    if(result > 0) return boost::optional<double>(result);
    return boost::optional<double>();
    
}

std::pair<boost::optional<double>,HitRecord> Sphere::hit(Ray r)
{
    auto dist = hitDistance(r);
    auto hit = HitRecord();
    if(dist)
    {
        hit.material = m;
        hit.at = r.origin + (*dist) * r.direction;
        hit.solid = hit.at;
        hit.n = hit.at - center(r.time);
        hit.n.normalize();
        hit.r = r;
        hit.u = 0.5 + atan2(hit.n.z, hit.n.x)/(2*M_PI);
        hit.v = 0.5 - asin(hit.n.y)/M_PI;
    }
    return std::make_pair(dist, hit);
}