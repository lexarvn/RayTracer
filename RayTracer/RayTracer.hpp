#ifndef RAYTRACER_HPP
#define RAYTRACER_HPP
#include "bitmap_image.hpp"
#include "Preprocess.hpp"
#include "Scene.hpp"
#include "Eye.hpp"
#include "Vector.hpp"
#include "Optional.hpp"
#include <memory>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <boost/optional.hpp>

#ifdef DO_PARALLEL
#include <amp.h>
#endif

class RayTracer: public Shape
{
public:
    static std::shared_ptr<RayTracer> getInstance()
    {
        if (!instance)
            instance = std::shared_ptr<RayTracer>(new RayTracer());
        return instance;
    }
	void addLight(std::shared_ptr<Shape> s);
	void addObject(std::shared_ptr<Shape> s);
    void setEye(Eye e);
    void renderScene();
	virtual boost::optional<double> hitDistance(Ray r);
	virtual std::pair<boost::optional<double>,HitRecord> hit(Ray r);
    unsigned int maxTime, maxRays;
    std::vector<std::shared_ptr<Shape>> getLights();
    virtual Vector getCenter(double);
	virtual Vector getRandomPoint(Vector,double);
private:
    RayTracer():Shape(std::make_shared<NullMaterial>(),BoundingBox()),objects(new Scene()),lights(new Scene()),eye(),maxTime(0),maxRays(1){}
    static std::shared_ptr<RayTracer> instance;
	Scene *objects, *lights;
	Optional<Eye> eye;
	Color renderPixel(int x, int y, double t);
};

#endif