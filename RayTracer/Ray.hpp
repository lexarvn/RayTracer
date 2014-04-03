#ifndef CS6400_RAY_HPP
#define CS6400_RAY_HPP

#include "Vector.hpp"

struct Ray
{
	Ray():origin(0,0,0),direction(0,0,0){}
	Vector origin;
	Vector direction;
    int depth;
    double time;
};

#endif
