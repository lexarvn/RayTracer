#include "MeshTriangle.hpp"

namespace
{
    double min(double a,double b)
    {
        return ((a<b)?a:b);
    }
    double max(double a,double b)
    {
        return ((a>b)?a:b);
    }
}


MeshTriangle::MeshTriangle(Vector a, Vector b, Vector c, Vector an, Vector bn, Vector cn, std::shared_ptr<Material> m)
:Shape(m,BoundingBox(min(a.x,min(b.x,c.x))
                     ,max(a.x,max(b.x,c.x))
                     ,min(a.y,min(b.y,c.y))
                     ,max(a.y,max(b.y,c.y))
                     ,min(a.z,min(b.z,c.z))
                     ,max(a.z,max(b.z,c.z))
                     )
       ),a(a),b(b),c(c),an(an),bn(bn),cn(cn){}

Vector MeshTriangle::getCenter(double t)
{
    auto cX = (a.x+b.x+c.x)/3;
    auto cY = (a.y+b.y+c.y)/3;
    auto cZ = (a.z+b.z+c.z)/3;
    return Vector(cX, cY, cZ);
}

Vector MeshTriangle::getRandomPoint(Vector n, double t)
{
    static std::default_random_engine g;
    static std::uniform_real_distribution<double> d(0,1);
	auto v1 = b-a;
	auto v2 = c-a;
    
	auto p = d(g)*v1 + d(g)*v2;
    
	if(p.length()>1)
		p = v1+v2-p;
    
	return p+a;
}

boost::optional<double> MeshTriangle::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    
    auto v1t = a-b;
    auto v2t = a-c;
    auto vot = a-r.origin;
    
    auto x2 = v2t.cross(r.direction);
    
    auto denom = v1t.dot(x2);
    beta = vot.dot(x2)/denom;
    
    if(beta < 0.0 || beta > 1.0)
        return boost::optional<double>();
    
    auto x1 = v1t.cross(vot);
    
    gamma = r.direction.dot(x1)/denom;
    if(gamma < 0.0 || gamma > 1.0 || beta+gamma > 1.0)
        return boost::optional<double>();
    
    auto dist = -(v2t.dot(x1))/denom;
    if(dist > 0.0)
        return boost::optional<double>(dist);
    return boost::optional<double>();
    
}

std::pair<boost::optional<double>,HitRecord> MeshTriangle::hit(Ray r)
{
    auto dist = hitDistance(r);
    auto hit = HitRecord();
    if(dist)
    {
        hit.material = m;
        hit.at = r.origin + (*dist) * r.direction;
        hit.solid = hit.at;
        hit.n = an * (1-beta-gamma) + bn * beta + cn * gamma;
        hit.n.normalize();
        hit.r = r;
        hit.u = beta;
        hit.v = gamma;
    }
    return std::make_pair(dist, hit);
}