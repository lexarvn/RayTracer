#include "Shape.hpp"

const double BoundingBox::EPSILON = 0.000001;

BoundingBox::BoundingBox(BoundingBox a, BoundingBox b)
:xmin(min(a.xmin,b.xmin)-EPSILON)
,xmax(max(a.xmax,b.xmax)+EPSILON)
,ymin(min(a.ymin,b.ymin)-EPSILON)
,ymax(max(a.ymax,b.ymax)+EPSILON)
,zmin(min(a.zmin,b.zmin)-EPSILON)
,zmax(max(a.zmax,b.zmax)+EPSILON)
{}

BoundingBox::BoundingBox(Keyframed<double> xmin, Keyframed<double> xmax, Keyframed<double> ymin, Keyframed<double> ymax, Keyframed<double> zmin, Keyframed<double> zmax)
:xmin(xmin-EPSILON)
,xmax(xmax+EPSILON)
,ymin(ymin-EPSILON)
,ymax(ymax+EPSILON)
,zmin(zmin-EPSILON)
,zmax(zmax+EPSILON)
{}

BoundingBox::BoundingBox():xmin(), xmax(), ymin(), ymax(), zmin(), zmax(){}

bool BoundingBox::hit(Ray r)
{
    auto txmin = (xmin(r.time) - r.origin.x)/r.direction.x;
    auto txmax = (xmax(r.time) - r.origin.x)/r.direction.x;
    if(txmin > txmax)
        std::swap(txmin,txmax);
    
    auto tymin = (ymin(r.time) - r.origin.y)/r.direction.y;
    auto tymax = (ymax(r.time) - r.origin.y)/r.direction.y;
    if(tymin > tymax)
        std::swap(tymin,tymax);
    
    if ((txmin > tymax) || (tymin > txmax))
        return false;
    
    auto tzmin = (zmin(r.time) - r.origin.z)/r.direction.z;
    auto tzmax = (zmax(r.time) - r.origin.z)/r.direction.z;
    if(tzmin > tzmax)
        std::swap(tzmin,tzmax);
    
    if ((std::max(txmin, tymin) > tzmax) || (tzmin > std::min(txmax, tymax)))
        return false;
    
    return true;
}

BoundingBox Shape::getBoundingBox()
{
    return bbox;
}

Shape::Shape(std::shared_ptr<Material> m, BoundingBox b):m(m),bbox(b){}
