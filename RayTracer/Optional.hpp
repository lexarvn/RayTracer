#ifndef CS6400_OPTIONAL_HPP
#define CS6400_OPTIONAL_HPP

template<typename T>
class Optional
{
public:
	Optional():t(),good(false){}
	Optional(T t):t(t),good(true){}
	operator bool() const{return good;}
	T operator*() const {return t;}
private:
	T t;
	bool good;
};


#endif