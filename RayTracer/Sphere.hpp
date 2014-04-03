#ifndef CS6400_SPHERE_HPP
#define CS6400_SPHERE_HPP

#include "Shape.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Keyframed.hpp"
#include <boost/optional.hpp>
#include <cmath>
#include <random>

class Sphere : public Shape
{
public:
	Sphere(Keyframed<Vector> center, Keyframed<double> radius, std::shared_ptr<Material> m);
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
private:
	Keyframed<Vector> center;
	Keyframed<double> radius;
};

#endif