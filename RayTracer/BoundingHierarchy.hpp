#ifndef BOUNDING_HIERARCHY_HPP
#define BOUNDING_HIERARCHY_HPP

#include "Shape.hpp"
#include <memory>
#include <vector>
#include <numeric>
#include <random>
#include <boost/optional.hpp>

class BoundingHierarchy : public Shape
{
protected:
    enum sorter {SORT_X,SORT_Y,SORT_Z};
	std::shared_ptr<Shape> s1, s2;
public:
	BoundingHierarchy(std::vector<std::shared_ptr<Shape>> v, sorter s=SORT_X);
	BoundingHierarchy(std::shared_ptr<Shape> s1, std::shared_ptr<Shape> s2);
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
};

#endif