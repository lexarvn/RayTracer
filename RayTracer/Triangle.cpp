#include "Triangle.hpp"

Triangle::Triangle(Keyframed<Vector> a, Keyframed<Vector> b, Keyframed<Vector> c, std::shared_ptr<Material> m)
:Shape(m,BoundingBox(min(getKeyframedX(a),min(getKeyframedX(b),getKeyframedX(c)))
                    ,max(getKeyframedX(a),max(getKeyframedX(b),getKeyframedX(c)))
                    ,min(getKeyframedY(a),min(getKeyframedY(b),getKeyframedY(c)))
                    ,max(getKeyframedY(a),max(getKeyframedY(b),getKeyframedY(c)))
                    ,min(getKeyframedZ(a),min(getKeyframedZ(b),getKeyframedZ(c)))
                    ,max(getKeyframedZ(a),max(getKeyframedZ(b),getKeyframedZ(c)))))
        ,a(a)
        ,b(b)
        ,c(c){}


Vector Triangle::getCenter(double t)
{
    auto at = a(t);
    auto bt = b(t);
    auto ct = c(t);
    auto cX = (at.x+bt.x+ct.x)/3;
    auto cY = (at.y+bt.y+ct.y)/3;
    auto cZ = (at.z+bt.z+ct.z)/3;
    return Vector(cX, cY, cZ);
}

Vector Triangle::getRandomPoint(Vector n, double t)
{
    static std::default_random_engine g;
    static std::uniform_real_distribution<double> d(0,1);
	auto v1 = b(t)-a(t);
	auto v2 = c(t)-a(t);

	auto p = d(g)*v1 + d(g)*v2;

	if(p.length()>1)
		p = v1+v2-p;

	return p+a(t);
}

boost::optional<double> Triangle::hitDistance(Ray r)
{
    if(!bbox.hit(r))
        return boost::optional<double>();
    
    auto v1t = a(r.time)-b(r.time);
    auto v2t = a(r.time)-c(r.time);
    auto vot = a(r.time)-r.origin;
    
    auto x2 = v2t.cross(r.direction);
    
    auto denom = v1t.dot(x2);
    beta = vot.dot(x2)/denom;
    
    if(beta < 0.0 || beta > 1.0)
        return boost::optional<double>();
    
    auto x1 = v1t.cross(vot);
    
    gamma = r.direction.dot(x1)/denom;
    if(gamma < 0.0 || gamma > 1.0 || beta+gamma > 1.0)
        return boost::optional<double>();
    
    auto dist = -(v2t.dot(x1))/denom;
    if(dist > 0.0)
        return boost::optional<double>(dist);
    return boost::optional<double>();
    
}

std::pair<boost::optional<double>,HitRecord> Triangle::hit(Ray r)
{
    auto dist = hitDistance(r);
    auto hit = HitRecord();
    if(dist)
    {
        hit.material = m;
        hit.at = r.origin + (*dist) * r.direction;
        hit.solid = hit.at;
        hit.n = (b(r.time)-a(r.time)).cross(c(r.time)-a(r.time));
        hit.n.normalize();
        hit.r = r;
        hit.u = beta;
        hit.v = gamma;
    }
    return std::make_pair(dist, hit);
}