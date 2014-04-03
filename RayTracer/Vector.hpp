#ifndef CS6400_VECTOR_HPP
#define CS6400_VECTOR_HPP

#include <cmath>
#include <iostream>

struct Vector
{
	double x;
	double y;
	double z;
	Vector():x(0),y(0),z(0){}
	Vector(double x,double y, double z):x(x),y(y),z(z){}
	Vector(double e):x(e),y(e),z(e){}
	Vector& operator+=(Vector v)
	{
		x+=v.x;
		y+=v.y;
		z+=v.z;
		return *this;
	}
	Vector& operator-=(Vector v)
	{
		x-=v.x;
		y-=v.y;
		z-=v.z;
		return *this;
	}
	Vector& operator*=(double d)
	{
		x*=d;
		y*=d;
		z*=d;
		return *this;
	}
	double dot(Vector v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}
    Vector cross(Vector v) const
	{
        Vector c;
        
        c.x = y * v.z - z * v.y;
		c.y = z * v.x - x * v.z;
		c.z = x * v.y - y * v.x;
        
		return c;
	}
    double length()
    {
		return std::sqrt(x*x + y*y + z*z);
    }
	void normalize()
	{
		auto l = length();
		if(l > 0.0)
        {
            x/=l;
            y/=l;
            z/=l;
            return;
        }
        std::cout << "Warning: normalized Vector(0,0,0)" << std::endl;
	}
};

inline bool operator<(const Vector a,const Vector b)
{
	if(a.x == b.x)
    {
        if(a.y == b.y)
        {
            return a.z < b.z;
        }
        return a.y < b.y;
    }
    return a.x < b.x;
}

inline bool operator==(const Vector a, const Vector b)
{
	if(a.x == b.x && a.y == b.y && a.z == b.z)
        return true;
    return false;
}

inline Vector operator-(const Vector a)
{
	Vector c(a);
	c.x = -c.x;
	c.y = -c.y;
	c.z = -c.z;
	return c;
}

inline Vector operator+(const Vector a,const Vector b)
{
	Vector c(a);
	c+=b;
	return c;
}

inline Vector operator-(const Vector a,const Vector b)
{
	Vector c(a);
	c-=b;
	return c;
}


inline Vector operator*(const Vector a,const double b)
{
	Vector c(a);
	c*=b;
	return c;
}

inline Vector operator*(const double b,const Vector a)
{
	Vector c(a);
	c*=b;
	return c;
}

#endif
