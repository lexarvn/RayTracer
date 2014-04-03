#include "Instance.hpp"

Instance::Instance(Keyframed<Vector> t, Keyframed<double> rx, Keyframed<double> ry, Keyframed<double> rz, Keyframed<Vector> s, std::shared_ptr<Shape> i):Shape(std::make_shared<NullMaterial>(),BoundingBox()),t(t),rx(rx),ry(ry),rz(rz),s(s),i(i),memo()
{
    transformBoundingBox();
}

std::pair<Matrix, Matrix> Instance::makeMatrices(const double time)
{
    auto it = memo.find(time);
    if (it != memo.end())
        return it->second;
    
    static const double degreesToRadians = M_PI/180.0;
    
    auto tt = t(time);
    auto st = s(time);
    
    auto m = translate(tt.x,tt.y,tt.z);
    m *= rotateX(rx(time)*degreesToRadians);
    m *= rotateY(ry(time)*degreesToRadians);
    m *= rotateZ(rz(time)*degreesToRadians);
    m *= scale(st.x,st.y,st.z);
    
    auto n = m.getInverse();
    
    auto entry = std::make_pair(time, std::make_pair(m, n));
    memo.insert(entry);
    return entry.second;
}

void Instance::transformBoundingBox()
{
    auto b = i->getBoundingBox();
    
    BoundingBox bb;
    
    for(int i = 0; i <= RayTracer::getInstance()->maxTime; ++i)
    {
        std::set<double> x,y,z;
        
        auto m = makeMatrices(i);
        
        auto p = Vector(b.xmin(i),0,0);
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
        
        p = Vector(b.xmax(i),0,0);
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
        
        p = Vector(0,b.ymin(i),0);
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
        
        p = Vector(0,b.ymax(i),0);
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
        
        p = Vector(0,0,b.zmin(i));
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
        
        p = Vector(0,0,b.zmax(i));
        p = transformLoc(m.first, p);
        x.insert(p.x);
        y.insert(p.y);
        z.insert(p.z);
               
        bb.xmin.addFrame(i, *x.begin());
        bb.xmax.addFrame(i, *x.rbegin());
        bb.ymin.addFrame(i, *y.begin());
        bb.ymax.addFrame(i, *y.rbegin());
        bb.zmin.addFrame(i, *z.begin());
        bb.zmax.addFrame(i, *z.rbegin());
    }
    bbox = bb;
}

Vector Instance::getCenter(double t)
{
    auto m = makeMatrices(t);
    auto c = i->getCenter(t);
    return transformLoc(m.first, c);
}

Vector Instance::getRandomPoint(Vector n,double t)
{
    auto m = makeMatrices(t);
    auto n_ = transformVec(m.second, n);
    n_.normalize();
    auto p = i->getRandomPoint(n_,t);
    return transformLoc(m.first, p);
}

boost::optional<double> Instance::hitDistance(Ray r)
{
    return (hit(r)).first;
}

std::pair<boost::optional<double>,HitRecord> Instance::hit(Ray r)
{
    auto m = makeMatrices(r.time);
    Ray tr;
    tr.time = r.time;
    tr.depth = r.depth;
    tr.origin = transformLoc(m.second, r.origin);
    tr.direction = transformVec(m.second, r.direction);
    tr.direction.normalize();
    auto hit = i->hit(tr);
    if(hit.first)
    {
        hit.second.r = r;
        hit.second.at = transformLoc(m.first, hit.second.at);
        hit.second.n = transformVec(m.first, hit.second.n);
        hit.second.n.normalize();
        hit.first = (hit.second.at - r.origin).length();
    }
    return hit;
}

