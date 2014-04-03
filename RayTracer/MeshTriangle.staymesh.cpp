#include "MeshTriangle.staymesh.hpp"
#include "Mesh.staymesh.hpp"

namespace
{
    double min(double a, double b, double c)
    {
        if(a < b)
            return ((a < c) ? a : c);
        return ((a < b) ? a : b);
    }
    double max(double a, double b, double c)
    {
        if(a > b)
            return ((a > c) ? a : c);
        return ((a > b) ? a : b);
    }
}

MeshTriangle::MeshTriangle(int ai, int bi, int ci, std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat)
:Shape(mat,BoundingBox( min(m->getPoint(ai).x,m->getPoint(bi).x,m->getPoint(ci).x)
                       ,max(m->getPoint(ai).x,m->getPoint(bi).x,m->getPoint(ci).x)
                       ,min(m->getPoint(ai).y,m->getPoint(bi).y,m->getPoint(ci).y)
                       ,max(m->getPoint(ai).y,m->getPoint(bi).y,m->getPoint(ci).y)
                       ,min(m->getPoint(ai).z,m->getPoint(bi).z,m->getPoint(ci).z)
                       ,max(m->getPoint(ai).z,m->getPoint(bi).z,m->getPoint(ci).z)
                       )
       ),ai(ai),bi(bi),ci(ci),mesh(m){}


Vector MeshTriangle::getCenter(double t)
{
    auto at = mesh->getPoint(ai);
    auto bt = mesh->getPoint(bi);
    auto ct = mesh->getPoint(ci);
    auto cX = (at.x+bt.x+ct.x)/3;
    auto cY = (at.y+bt.y+ct.y)/3;
    auto cZ = (at.z+bt.z+ct.z)/3;
    return Vector(cX, cY, cZ);
}

Vector MeshTriangle::getRandomPoint(Vector n, double t)
{
    static std::default_random_engine g;
    static std::uniform_real_distribution<double> d(0,1);
	auto v1 = mesh->getPoint(bi)-mesh->getPoint(ai);
	auto v2 = mesh->getPoint(ci)-mesh->getPoint(ai);
    
	auto p = d(g)*v1 + d(g)*v2;
    
	if(p.length()>1)
		p = v1+v2-p;
    
	return p+mesh->getPoint(ai);
}

boost::optional<double> MeshTriangle::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    
    auto v1t = mesh->getPoint(ai)-mesh->getPoint(bi);
    auto v2t = mesh->getPoint(ai)-mesh->getPoint(ci);
    auto vot = mesh->getPoint(ai)-r.origin;
    
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
        hit.n = mesh->getNormal(ai) * (1-beta-gamma) + mesh->getNormal(bi) * beta + mesh->getNormal(ci) * gamma;
        hit.n.normalize();
        hit.r = r;
        hit.u = beta;
        hit.v = gamma;
    }
    return std::make_pair(dist, hit);
}