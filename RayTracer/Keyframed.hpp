#ifndef CS6400_KEYFRAMED_VALUE_HPP
#define CS6400_KEYFRAMED_VALUE_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <map>
#include "Vector.hpp"
#include "Matrix.hpp"

template<typename T>
class Keyframed
{
public:
	Keyframed():map(),memo(),min(-1),max(-1),lastTime(0){}
	Keyframed(T val):map(),memo(),min(0),max(0),lastTime(0)
    {
        map.push_back(entry(0,val));
    }
    
	T operator()(double t)
    {
        if(t < min)
            return this->operator()(min);
        if(t > max)
            return this->operator()(max);
        
        if(t-lastTime > .5)
            memo.clear();
        
        lastTime = t;
        
        auto it = memo.find(t);
        if(it != memo.end())
            return it->second;
        
        if(map.empty()) throw std::domain_error("No Key Frames");
        auto a = justLess(t);
        auto b = justMore(t);
        auto val = a.value;
        if((b.key - a.key)!=0)
        {
            auto weight = (t - a.key)/(b.key - a.key);
            val = a.value * (1-weight) + b.value * weight;
        }
        
        memo.insert(std::make_pair(t,val));
        return memo[t];
    }
	void addFrame(double t, T v)
    {
        memo.clear();
        map.push_back(entry(t,v));
        std::sort(map.begin(),map.end());
        min = map.front().key;
        max = map.back().key;
    }
    
    static std::set<double> getKeys(Keyframed<T> a, Keyframed<T> b)
    {
        std::set<double> keys;
        for(auto e:a.map)
        {
            keys.insert(e.key);
        }
        for(auto e:b.map)
        {
            keys.insert(e.key);
        }
        return keys;
    }
    
    static std::set<double> getKeys(Keyframed<T> a)
    {
        std::set<double> keys;
        for(auto e:a.map)
        {
            keys.insert(e.key);
        }
        return keys;
    }
    
    T average()
    {
        if(map.size() < 2)
            return map.front().value;
        
        std::sort(map.begin(),map.end());
        
        double diff = 0;
        double prevTime = -1;
        T prev = 0;
        T average = T(0);
        for(auto e:map)
        {
            if(prevTime != -1)
            {
                diff = e.key - prevTime;
                average += (prev + e.value)*diff;
            }
            prevTime = e.key;
            prev = e.value;
        }
        
        return average/map.size();
    }
    
private:
    struct entry
    {
        double key;
        T value;
        entry(double key, T value):key(key),value(value){}
        bool operator<(const entry e) const
        {
            return key < e.key;
        }
    };
	entry justLess(double t)
    {
        std::vector<entry> less;
        std::copy_if(map.begin()
                     ,map.end()
                     ,back_inserter(less)
                     ,[=](entry e){return e.key < t;});
        if(!less.empty()) return less.back();
        return map.front();
    }
	entry justMore(double t)
    {
        std::vector<entry> more;
        std::copy_if(map.begin()
                     ,map.end()
                     ,back_inserter(more)
                     ,[=](entry e){return e.key >= t;});
        if(!more.empty()) return more.front();
        return map.back();
    }
	std::vector<entry> map;
    std::map<double,T> memo;
    double min;
    double max;
    double lastTime;
    friend Keyframed<double> operator-(Keyframed<double> a, double b);
    friend Keyframed<double> operator+(Keyframed<double> a, double b);
    friend Keyframed<double> operator-(double b, Keyframed<double> a);
    friend Keyframed<double> operator+(double b, Keyframed<double> a);
    friend Keyframed<double> getKeyframedX(Keyframed<Vector> v);
    friend Keyframed<double> getKeyframedY(Keyframed<Vector> v);
    friend Keyframed<double> getKeyframedZ(Keyframed<Vector> v);
    friend Keyframed<Matrix> invert(Keyframed<Matrix>);
};

template<typename T>
inline Keyframed<T> min(Keyframed<T> a, Keyframed<T> b)
{
    Keyframed<T> result;
    auto keys = Keyframed<T>::getKeys(a,b);
    for(auto key:keys)
    {
        result.addFrame(key,std::min(a(key),b(key)));
    }
    return result;
}

template<typename T>
inline Keyframed<T> max(Keyframed<T> a, Keyframed<T> b)
{
    Keyframed<T> result;
    auto keys = Keyframed<T>::getKeys(a,b);
    for(auto key:keys)
    {
        result.addFrame(key,std::max(a(key),b(key)));
    }
    return result;
}

template<typename T>
inline Keyframed<T> operator-(Keyframed<T> a, Keyframed<T> b)
{
	Keyframed<T> result;
    auto keys = Keyframed<T>::getKeys(a,b);
    for(auto key:keys)
    {
        result.addFrame(key,a(key)-b(key));
    }
    return result;
}

template<typename T>
inline Keyframed<T> operator+(Keyframed<T> a, Keyframed<T> b)
{
	Keyframed<T> result;
    auto keys = Keyframed<T>::getKeys(a,b);
    for(auto key:keys)
    {
        result.addFrame(key,a(key)+b(key));
    }
    return result;
}

inline Keyframed<double> operator-(Keyframed<double> a, double b)
{
	Keyframed<double> c;
    for(auto e:a.map)
    {
        c.addFrame(e.key, e.value-b);
    }
    return c;
}

inline Keyframed<double> operator+(Keyframed<double> a, double b)
{
	Keyframed<double> c;
    for(auto e:a.map)
    {
        c.addFrame(e.key, e.value+b);
    }
    return c;
}

inline Keyframed<double> operator-(double b, Keyframed<double> a)
{
	Keyframed<double> c;
    for(auto e:a.map)
    {
        c.addFrame(e.key, b-e.value);
    }
    return c;
}

inline Keyframed<double> operator+(double b, Keyframed<double> a)
{
	Keyframed<double> c;
    for(auto e:a.map)
    {
        c.addFrame(e.key, e.value+b);
    }
    return c;
}

inline Keyframed<double> getKeyframedX(Keyframed<Vector> v)
{
    Keyframed<double> x;
    for(auto e:v.map)
    {
        x.addFrame(e.key,e.value.x);
    }
    return x;
}
inline Keyframed<double> getKeyframedY(Keyframed<Vector> v)
{
    Keyframed<double> y;
    for(auto e:v.map)
    {
        y.addFrame(e.key,e.value.y);
    }
    return y;
}
inline Keyframed<double> getKeyframedZ(Keyframed<Vector> v)
{
    Keyframed<double> z;
    for(auto e:v.map)
    {
        z.addFrame(e.key,e.value.z);
    }
    return z;
}

inline Keyframed<Matrix> invert(Keyframed<Matrix> m)
{
    Keyframed<Matrix> i;
    for(auto e:m.map)
    {
        i.addFrame(e.key, e.value.getInverse());
    }
    return i;
}

#endif