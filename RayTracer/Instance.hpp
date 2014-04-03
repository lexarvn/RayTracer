#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include "Shape.hpp"
#include "BoundingHierarchy.hpp"
#include "Matrix.hpp"
#include "RayTracer.hpp"
#include <memory>
#include <vector>
#include <map>
#include <boost/optional.hpp>

class Instance : public Shape
{
public:
	Instance(Keyframed<Vector> t, Keyframed<double> rx, Keyframed<double> ry, Keyframed<double> rz, Keyframed<Vector> s, std::shared_ptr<Shape> i);
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
protected:    
	std::shared_ptr<Shape> i;
    Keyframed<Vector> t;
    Keyframed<double> rx, ry, rz;
    Keyframed<Vector> s;
    std::map<double,std::pair<Matrix, Matrix>> memo;
private:
    std::pair<Matrix, Matrix> makeMatrices(const double time);
    void transformBoundingBox();
};

#endif