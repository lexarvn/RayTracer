#ifndef MESH_TRIANGLE_HPP
#define MESH_TRIANGLE_HPP

#include "Shape.hpp"
#include <boost/optional.hpp>

class MeshTriangle : public Shape
{
public:
	MeshTriangle(Vector a, Vector b, Vector c, Vector an, Vector bn, Vector cn, std::shared_ptr<Material> m);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
	virtual boost::optional<double> hitDistance(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
private:
    Vector a, b, c;
    double beta, gamma;
	Vector an, bn, cn;
};

#endif