#ifndef MESH_TRIANGLE_HPP
#define MESH_TRIANGLE_HPP

#include "Shape"
#include <memory>
#include <vector>
#include <boost/optional.hpp>

class Mesh;

class MeshTriangle : public Shape
{
public:
	MeshTriangle(int a, int b, int c, std::shared_ptr<Mesh> m, std::shared_ptr<Material> mat);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
	virtual boost::optional<double> hitDistance(Ray r);
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
private:
    std::shared_ptr<Mesh> mesh;
    int ai, bi, ci;
    double beta, gamma;
};

#endif