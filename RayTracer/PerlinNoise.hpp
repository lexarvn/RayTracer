#ifndef PERLIN_NOISE_HPP
#define PERLIN_NOISE_HPP

#include "Vector.hpp"

#include <cmath>
#include <map>
#include <random>

class PerlinNoise
{
public:
    PerlinNoise(double p, unsigned int o):persistence(p),octaves(o),memo(){}
    double operator()(Vector v);
private:
    double persistence;
    unsigned int octaves;
    std::map<Vector, double> memo;
};

#endif