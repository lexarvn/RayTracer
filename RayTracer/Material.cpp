#include "Material.hpp"
#include "RayTracer.hpp"
#include "HitRecord.hpp"

const double Material::EPSILON = 0.000001;

namespace
{
    double randomFromCosineDistribution( double xMin, double xMax )
    {
        if(xMin > xMax)
            std::swap(xMin,xMax);
        
        static std::default_random_engine generator;
        static std::uniform_real_distribution<double> distribution(-1.0,1.0);
        
        double a = 0.5 * ( xMin + xMax ); // location parameter
        double b = ( xMax - xMin ) / M_PI; // scale parameter
        return a + b * std::asin(distribution(generator));
    }
    
    bool rayHittingBackSide(Vector n, Ray r)
    {
        if(n.dot(-r.direction) < 0)
            return true;
        return false;
        
    }
}

Color NullMaterial::operator()(const HitRecord &)
{
	return Color(0,0,0);
}

Color DiffuseMaterial::operator()(const HitRecord &h)
{
#ifdef ADD_PATH_TRACE
    if(h.r.depth > MAX_DEPTH)
        return Color();
#endif
    if(h.r.depth > WAY_TOO_DEEP)
        return Color();
    
    auto n = h.n;
    if(rayHittingBackSide(n, h.r))
        n = -n;
    
    auto s = RayTracer::getInstance();

#ifndef ADD_PATH_TRACE
    auto lights = s->getLights();
    
    auto colorAverage = Color();
    
    for(auto l:lights)
    {
#ifdef ADD_AREA_LIGHTS
        auto light = l->getRandomPoint(h.n,h.r.time);
#else
		auto light = l->getCenter(h.r.time);
#endif

#ifdef ADD_SHADOW_RAY
    
        Ray r2;
        r2.time = h.r.time;
        r2.depth = h.r.depth+1;
        r2.origin = h.at + (n*EPSILON);
        r2.direction = light - r2.origin;
        auto lightDistance = r2.direction.length();
        r2.direction.normalize();
        
        auto dist = s->hitDistance(r2);
        
        if(*dist < lightDistance)
            continue;
#endif
        
        auto lightDirection = light - h.at;
        lightDirection.normalize();
        auto scale = n.dot(lightDirection);
        if(scale < 0) scale = 0;
        if(scale > 1) scale = 1;
        colorAverage += (color->at(h.u,h.v) * scale * l->m->operator()(h));
    }
    
    colorAverage /= lights.size();
    return colorAverage;
    
#endif
    
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> dist(-1.0,1.0);
    
    Ray l;
    l.time = h.r.time;
    l.depth = h.r.depth+1;
    l.origin = h.at + (n*EPSILON);
    auto u = dist(generator);
    auto theta = (dist(generator)+1)*M_PI;
    l.direction = Vector(sqrt(1 - u*u) * cos(theta),sqrt(1 - u*u) * sin(theta),u);
    if(l.direction.dot(n) < 0)
        l.direction = -l.direction;

    
    l.direction.normalize();
    auto hit = s->hit(l);
    return color->at(h.u,h.v) * hit.second.getColor();
}

Color SpecularMaterial::operator()(const HitRecord &h)
{
#ifdef ADD_PATH_TRACE
    if(h.r.depth > MAX_DEPTH)
        return Color();
#endif
    if(h.r.depth > WAY_TOO_DEEP)
        return Color();
    
    auto n = h.n;
    if(rayHittingBackSide(n, h.r))
        n = -n;
    Ray r2;
    r2.time = h.r.time;
    r2.depth = h.r.depth+1;
    r2.origin = h.at + (n*EPSILON);
    auto a = (h.r.direction.dot(n))*n;
    r2.direction = h.r.direction - 2*a;
    r2.direction.normalize();
    
#ifdef ADD_SPECULAR_JITTER
    auto dma = h.r.direction - a;
    
    Vector u = r2.direction.cross(n);
    if(u.length() > EPSILON)
    {
        u.normalize();

        Vector v = u.cross(r2.direction);
        v.normalize();
        
        auto limit = std::abs(std::sqrt(a.dot(a))/a.dot(v));
        auto temp = std::abs(std::sqrt(dma.dot(dma))/dma.dot(v));
        if(temp < limit)
            limit = temp;
        r2.direction += v*limit*variancePercent*randomFromCosineDistribution(-1, 1) + u*limit*variancePercent*randomFromCosineDistribution(-1, 1);
    }
#endif
    
    r2.direction.normalize();
    auto s = RayTracer::getInstance();
    auto hit = s->hit(r2);
    return color->at(h.u,h.v) * hit.second.getColor();
}

Color DielectricMaterial::operator()(const HitRecord &h)
{
#ifdef ADD_PATH_TRACE
    if(h.r.depth > MAX_DEPTH)
        return Color();
#endif
    if(h.r.depth > WAY_TOO_DEEP)
        return Color();
    
    auto n = h.n;
    auto ri = 1/refactiveIndex;
    if(rayHittingBackSide(n, h.r))
    {
        n = -n;
        ri = refactiveIndex;
    }
    Ray r2;
    r2.time = h.r.time;
    r2.depth = h.r.depth+1;
    auto ndd = n.dot(h.r.direction);
    auto underSqrt = 1-(1-ndd*ndd)*(ri*ri);
    auto a = (h.r.direction.dot(n))*n;
    if(underSqrt > 0)
    {
        r2.origin = h.at + (-n*EPSILON);
        r2.direction = (h.r.direction-n*ndd)*ri - n*std::sqrt(underSqrt);
    }
    else
    {
        r2.origin = h.at + (n*EPSILON);
        r2.direction = h.r.direction - 2*a;
    }
    
#ifdef ADD_DIELECTRIC_JITTER
    auto dma = h.r.direction - a;
    
    Vector u = r2.direction.cross(n);
    if(u.length() > EPSILON)
    {
        u.normalize();
    
        Vector v = u.cross(r2.direction);
        v.normalize();
        
        auto limit = std::abs(std::sqrt(a.dot(a))/a.dot(v));
        auto temp = std::abs(std::sqrt(dma.dot(dma))/dma.dot(v));
        if(temp < limit)
            limit = temp;
        r2.direction += v*limit*variancePercent*randomFromCosineDistribution(-1, 1) + u*limit*variancePercent*randomFromCosineDistribution(-1, 1);
    }
#endif
    
    r2.direction.normalize();
    auto s = RayTracer::getInstance();
    auto hit = s->hit(r2);
    return color->at(h.u,h.v) * hit.second.getColor();
}

Color EmissiveMaterial::operator()(const HitRecord &h)
{
    return color->at(h.u,h.v);
}

Color CheckeredMaterial::operator()(const HitRecord &h)
{
	int z = static_cast<int>(floor(h.solid.x/size)+floor(h.solid.y/size)+floor((h.solid.z)/size));
	if(z%2==0)
	{
		if(a) return a->operator()(h);
	}
	else
	{
		if(b) return b->operator()(h);
	}
	return Color();
}

Color CompoundMaterial::operator()(const HitRecord &h)
{
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> uDist(0,aWeight+bWeight);
	if(uDist(generator) <= aWeight)
	{
		if(a) return a->operator()(h);
	}
	else
	{
		if(b) return b->operator()(h);
	}
	return Color();
}

Color TurbulentMaterial::operator()(const HitRecord &h)
{
    auto h_ = HitRecord(h);
    static auto p = PerlinNoise(.25, 4);
    
	h_.at = h.at + Vector(p(h.at));
	h_.solid = h.solid + Vector(p(h.solid));
	auto uv = Vector(h.u,h.v,0) + Vector(p(Vector(h.u,h.v,0)));
    if(uv.x < 0)
        uv.x=0;
    else if(uv.x > 1)
        uv.x=1;
    if(uv.y < 0)
        uv.y=0;
    else if(uv.y > 1)
        uv.y=1;
    h_.u = uv.x;
    h_.v = uv.y;
	if(a) return a->operator()(h_);
	return Color();
}

Color MarbleMaterial::operator()(const HitRecord &h)
{
    static auto p = PerlinNoise(.15, 4);
    auto noise = p(h.solid);
    auto solid = h.solid + Vector(noise);
    return m->operator()(h)*std::abs(cos(solid.x + solid.y + solid.z + strength*noise));
}
