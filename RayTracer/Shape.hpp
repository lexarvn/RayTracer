#ifndef CS6400_SHAPE_HPP
#define CS6400_SHAPE_HPP

#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Keyframed.hpp"
#include "Matrix.hpp"
#include <boost/optional.hpp>


struct BoundingBox
{
    static const double EPSILON;
    Keyframed<double> xmin, xmax, ymin, ymax, zmin, zmax;
    BoundingBox(BoundingBox a, BoundingBox b);
    
    BoundingBox(Keyframed<double> xmin, Keyframed<double> xmax, Keyframed<double> ymin, Keyframed<double> ymax, Keyframed<double> zmin, Keyframed<double> zmax);
    
    BoundingBox();
    
    bool hit(Ray r);
};

class Shape
{
public:
	Shape(std::shared_ptr<Material> m, BoundingBox b);
	virtual boost::optional<double> hitDistance(Ray)=0;
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray)=0;
	virtual Vector getCenter(double)=0;
	virtual Vector getRandomPoint(Vector,double)=0;
    BoundingBox getBoundingBox();
	std::shared_ptr<Material> m;
protected:
    BoundingBox bbox;
};

#endif