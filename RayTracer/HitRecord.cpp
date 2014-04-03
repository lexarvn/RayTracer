#include "HitRecord.hpp"

Color HitRecord::getColor()
{
    if(material)
    {
        return material->operator()(*this);
    }
    else
    {
        return Color();
    }
}