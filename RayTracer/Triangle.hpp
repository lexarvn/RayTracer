#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Shape.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Keyframed.hpp"
#include <boost/optional.hpp>

class Triangle : public Shape
{
public:
	Triangle(Keyframed<Vector> a, Keyframed<Vector> b, Keyframed<Vector> c, std::shared_ptr<Material> m);
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
protected:
	Keyframed<Vector> a, b, c;
    double beta, gamma;
};

#endif