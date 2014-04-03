#include "Scene.hpp"

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
}

void Scene::buildHierarchy()
{
	if(shapes.size() > 2)
	{
		auto h = std::make_shared<BoundingHierarchy>(shapes);
		shapes.clear();
		add(h);
	}
}

Scene::Scene():Shape(std::shared_ptr<Material>(),BoundingBox()),shapes(){}

Vector Scene::getCenter(double t)
{
    auto cX = (bbox.xmax(t) + bbox.xmin(t))/2;
    auto cY = (bbox.ymax(t) + bbox.ymin(t))/2;
    auto cZ = (bbox.zmax(t) + bbox.zmin(t))/2;
    return Vector(cX, cY, cZ);
}

Vector Scene::getRandomPoint(Vector n, double t)
{
    static std::default_random_engine generator;
    static std::uniform_int_distribution<int> distribution(0,static_cast<int>(shapes.size()-1));
    return shapes[distribution(generator)]->getRandomPoint(n, t);
}

boost::optional<double> Scene::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    boost::optional<double> dist;
    for(auto& s:shapes)
    {
        auto hit = s->hitDistance(r);
        auto m = min(dist,hit);
        if(m != dist)
        {
            dist = m;
        }
    }
    return dist;
}

std::pair<boost::optional<double>,HitRecord> Scene::hit(Ray r)
{
    auto hit = std::make_pair(boost::optional<double>(), HitRecord());
//    std::cout << r.depth << std::endl;
    if(bbox.hit(r)){
        for(auto& s:shapes)
        {
            auto cHit = s->hit(r);
            auto m = min(hit.first,cHit.first);
            if(m != hit.first)
            {
                hit = cHit;
            }
        }
    }
    return hit;
}

void Scene::add(std::shared_ptr<Shape> s)
{
    shapes.push_back(s);
    recalculateBoundingBox();
}

void Scene::recalculateBoundingBox()
{
    bbox = BoundingBox(shapes.front()->getBoundingBox());
    for(auto& s:shapes)
    {
        bbox = BoundingBox(bbox,s->getBoundingBox());
    }
}
