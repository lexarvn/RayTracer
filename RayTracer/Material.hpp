#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Preprocess.hpp"
#include "Vector.hpp"
#include "Ray.hpp"
#include "bitmap_image.hpp"
#include "PerlinNoise.hpp"

#include <cmath>
#include <memory>
#include <random>

struct HitRecord;

struct Color
{
	double r,g,b;
	Color():r(0),g(0),b(0){}
	Color(double r,double g,double b):r(r),g(g),b(b){}
    double max()
    {
        if(g >= b)
        {
            return ((r >= g) ? r : g);
        }
        return ((r >= b) ? r : b);
    }
    double min()
    {
        if(g < b)
        {
            return ((r < g) ? r : g);
        }
        return ((r < b) ? r : b);
    }
	Color & operator*=(double s)
	{
		r*=s;
		g*=s;
		b*=s;
		return *this;
	}
	Color & operator/=(double s)
	{
		r/=s;
		g/=s;
		b/=s;
		return *this;
	}
	Color& operator+=(Color c)
	{
		r+=c.r;
		g+=c.g;
		b+=c.b;
		return *this;
	}
	Color& operator-=(Color c)
	{
		r-=c.r;
		g-=c.g;
		b-=c.b;
		return *this;
	}
    void cutoff(double cut)
    {
        r = cutoff(r, cut);
        g = cutoff(g, cut);
        b = cutoff(b, cut);
    }
private:
    double cutoff(double value,double cut)
    {
        if(value > cut)
            return cut;
        if(value < 0.0)
            return 0.0;
        return value;
    }
};

inline Color operator+ (const Color a, const Color b)
{
	Color c(a);
	c+=b;
	return c;
}
inline Color operator- (const Color a, const Color b)
{
	Color c(a);
	c-=b;
	return c;
}

inline Color operator* (const Color a, const double b)
{
	Color c(a);
	c*=b;
	return c;
}

inline Color operator* ( const double b,const Color a)
{
	Color c(a);
	c*=b;
	return c;
}

inline Color operator/ (const Color a, const double b)
{
	Color c(a);
	c/=b;
	return c;
}

inline Color operator* (const Color a, const Color b)
{
	Color c(a);
	c.r *= b.r;
	c.g *= b.g;
	c.b *= b.b;
	return c;
}

struct ColorInterface
{
    virtual Color at(double, double)=0;
};

struct SolidColor:ColorInterface
{
	SolidColor():ColorInterface(),c(){}
	SolidColor(double r,double g,double b):ColorInterface(),c(r,g,b){}
    virtual Color at(double, double)
    {
        return c;
    }
private:
    Color c;
};

struct ColorMap:ColorInterface
{
    ColorMap(std::string filename):ColorInterface(),bmp(std::make_shared<bitmap_image>(filename))
    {
		std::cout << "Loading " << filename << std::endl;
    }
    virtual Color at(double u, double v)
    {
        auto x = bmp->width()-(u * (bmp->width()-2)+.5);
        auto y = v * (bmp->height()-2)+.5;
                
        int x1 = static_cast<int>(floor(x));
        int x2 = x1+1;
        int y1 = static_cast<int>(floor(y));
        int y2 = y1+1;
        
        unsigned char rt,gt,bt;
        bmp->get_pixel(x1, y1, rt, gt, bt);
        auto p11 = Color(rt/256.0, gt/256.0, bt/256.0);
        bmp->get_pixel(x1, y2, rt, gt, bt);
        auto p12 = Color(rt/256.0, gt/256.0, bt/256.0);
        bmp->get_pixel(x2, y1, rt, gt, bt);
        auto p21 = Color(rt/256.0, gt/256.0, bt/256.0);
        bmp->get_pixel(x2, y2, rt, gt, bt);
        auto p22 = Color(rt/256.0, gt/256.0, bt/256.0);
        
        auto p = p11*(x2-x)*(y2-y)+p21*(x-x1)*(y2-y)+p12*(x2-x)*(y-y1)+p22*(x-x1)*(y-y1);
            
        return p;
    }

private:
    std::shared_ptr<bitmap_image> bmp;
};

struct Material
{
	virtual Color operator()(const HitRecord &)=0;
    static const double EPSILON;
};

struct NullMaterial:Material
{
	virtual Color operator()(const HitRecord &);
};

struct DiffuseMaterial:Material
{
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<ColorInterface> color;
	DiffuseMaterial(std::shared_ptr<ColorInterface> color):color(color){}
};

struct SpecularMaterial:Material
{
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<ColorInterface> color;
    double variancePercent;
	SpecularMaterial(std::shared_ptr<ColorInterface> c):color(c),variancePercent(0){}
    SpecularMaterial(std::shared_ptr<ColorInterface> c, double v):color(c),variancePercent(v)
    {
        if(variancePercent > 1)
            variancePercent = 1;
        if(variancePercent < 0)
            variancePercent = 0;
    }
};

struct DielectricMaterial:Material
{
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<ColorInterface> color;
    double refactiveIndex;
    double variancePercent;
    DielectricMaterial(std::shared_ptr<ColorInterface> c, double r):color(c),refactiveIndex(r),variancePercent(0){}
	DielectricMaterial(std::shared_ptr<ColorInterface> c, double r, double v):color(c),refactiveIndex(r),variancePercent(v)
    {
        if(variancePercent > 1)
            variancePercent = 1;
        if(variancePercent < 0)
            variancePercent = 0;
    }
};

struct EmissiveMaterial:Material
{
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<ColorInterface> color;
	EmissiveMaterial(std::shared_ptr<ColorInterface> color):color(color){}
};

struct CheckeredMaterial:Material
{
	CheckeredMaterial(std::shared_ptr<Material> a,std::shared_ptr<Material> b,double size):a(a),b(b),size(size)
	{if(size <= 0.0) throw std::domain_error("Size can not be zero");}
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<Material> a;
	std::shared_ptr<Material> b;
	double size;
};

struct CompoundMaterial:Material
{
	CompoundMaterial(std::shared_ptr<Material> a, double aw,std::shared_ptr<Material> b, double bw):a(a),b(b),aWeight(aw),bWeight(bw){}
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<Material> a;
	std::shared_ptr<Material> b;
	double aWeight, bWeight;
};

struct TurbulentMaterial:Material
{
	TurbulentMaterial(std::shared_ptr<Material> a):a(a){}
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<Material> a;
};

struct MarbleMaterial:Material
{
	MarbleMaterial(std::shared_ptr<Material> m, double s):m(m),strength(s){}
	virtual Color operator()(const HitRecord &);
	std::shared_ptr<Material> m;
    double strength;
};

#endif