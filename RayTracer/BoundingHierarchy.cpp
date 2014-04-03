#include "BoundingHierarchy.hpp"

namespace
{
    bool min(const double a, const double b)
	{
		//Must be positive
		if(a < 0) return false;
		if(b < 0) return true;
		if(a < b) return true;
		return false;
	}
    
	boost::optional<double> min(const boost::optional<double> a,const boost::optional<double> b)
	{
		//Must be valid
		if(!a) return b;
		if(!b) return a;
		return min(*a,*b)?a:b;
	}
    
    bool compareX(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2)
    {
        return s1->getBoundingBox().xmin.average() < s2->getBoundingBox().xmin.average();
    }
    
    bool compareY(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2)
    {
        return s1->getBoundingBox().ymin.average() < s2->getBoundingBox().ymin.average();
    }
    
    bool compareZ(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2)
    {
        return s1->getBoundingBox().zmin.average() < s2->getBoundingBox().zmin.average();
    }
}

BoundingHierarchy::BoundingHierarchy(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2):Shape(std::shared_ptr<Material>(),BoundingBox(s1->getBoundingBox(),s2->getBoundingBox())),s1(s1),s2(s2){}

BoundingHierarchy::BoundingHierarchy(std::vector<std::shared_ptr<Shape>> v, sorter s):Shape(std::shared_ptr<Material>(),BoundingBox())
{
    std::function<bool(std::shared_ptr<Shape>,std::shared_ptr<Shape>)> comp;
    if(s == SORT_X)
    {
        comp = compareX;
        s = SORT_Y;
    }
    else if(s == SORT_Y)
    {
        comp = compareY;
        s = SORT_Z;
    }
    else
    {
        comp = compareZ;
        s = SORT_X;
    }
        
    std::sort(v.begin(), v.end(), comp);
    
    if(v.size() == 3)
    {
        s1 = v[0];
        s2 = std::make_shared<BoundingHierarchy>(v[1],v[2]);
    }
    else if(v.size() < 3)
    {
        s1 = v[0];
        s2 = v[1];
    }
    else
    {
        std::vector<std::shared_ptr<Shape>> v1(v.begin(), v.begin() + v.size()/2),
        v2(v.begin() + v.size()/2, v.end());
        
        s1 = std::make_shared<BoundingHierarchy>(v1,s);
        s2 = std::make_shared<BoundingHierarchy>(v2,s);
    }
    bbox = BoundingBox(s1->getBoundingBox(), s2->getBoundingBox());
}

Vector BoundingHierarchy::getCenter(double t)
{
    auto cX = (bbox.xmax(t) + bbox.xmin(t))/2;
    auto cY = (bbox.ymax(t) + bbox.ymin(t))/2;
    auto cZ = (bbox.zmax(t) + bbox.zmin(t))/2;
    return Vector(cX, cY, cZ);
}

Vector BoundingHierarchy::getRandomPoint(Vector n, double t)
{
    static std::default_random_engine generator;
    static std::bernoulli_distribution distribution(0.5);
    if(distribution(generator))
        return s1->getRandomPoint(n, t);
    return s2->getRandomPoint(n, t);
}

boost::optional<double> BoundingHierarchy::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    boost::optional<double> dist;
    dist = min(s1->hitDistance(r), s2->hitDistance(r));
    return dist;
}

std::pair<boost::optional<double>,HitRecord> BoundingHierarchy::hit(Ray r)
{
    auto hit = std::make_pair(boost::optional<double>(), HitRecord());
    if(bbox.hit(r)){
        hit = s1->hit(r);
        auto temp = s2->hit(r);
        
        auto m = min(hit.first,temp.first);
        if(m != hit.first)
        {
            hit = temp;
        }
    }
    return hit;
}
