#ifndef CS6400_SCENE_HPP
#define CS6400_SCENE_HPP

#include "Shape.hpp"
#include "BoundingHierarchy.hpp"
#include <memory>
#include <vector>
#include <numeric>
#include <boost/optional.hpp>

class Scene : public Shape
{
public:
	Scene();
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
	void add(std::shared_ptr<Shape> s);
    void buildHierarchy();
	std::vector<std::shared_ptr<Shape>> shapes;
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
protected:    
    void recalculateBoundingBox();
};

#endif