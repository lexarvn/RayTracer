#include "RayTracer.hpp"

std::shared_ptr<RayTracer> RayTracer::instance = nullptr;

namespace
{
    double modulus(double a, double b)
    {
        int result = static_cast<int>( a / b );
        return a - static_cast<double>( result ) * b;
    }
}


void RayTracer::addLight(std::shared_ptr<Shape> s)
{
    lights->add(s);
#ifdef ADD_PATH_TRACE
    objects->add(s);
    return;
#endif
#ifdef ADD_AREA_LIGHTS
    objects->add(s);
#endif
}

Vector RayTracer::getCenter(double)
{
    return Vector();
}


Vector RayTracer::getRandomPoint(Vector,double)
{
    return Vector();
}

void RayTracer::addObject(std::shared_ptr<Shape> s)
{
    objects->add(s);
}

std::vector<std::shared_ptr<Shape>> RayTracer::getLights()
{
    return lights->shapes;
}

void RayTracer::setEye(Eye e)
{
    eye = Optional<Eye>(e);
}

Color RayTracer::renderPixel(int x, int y, double t)
{
	auto color = Color();
	for(unsigned int z = 0;z < maxRays;++z)
	{
		auto r = (*eye).makeRay(t, x, y);
		auto h = objects->hit(r);
		color += h.second.getColor();
	}
	return color;
}

void RayTracer::renderScene()
{
    if(!eye)
    {
        std::cout << "Eye must be set before rendering scene" << std::endl;
        return;
    }
    
    objects->buildHierarchy();
    
    const int WIDTH = (*eye).resolutionX;
	const int HEIGHT = (*eye).resolutionY;
    
#ifdef ADD_HDR
    const int NUMBER_OF_PIXELS = WIDTH*HEIGHT;
#endif
    
    std::vector<std::vector<Color>> image;
    image.resize(WIDTH);
    for(auto &e:image)
    {
        e.resize(HEIGHT);
    }
  
    if(maxRays == 0)
        maxRays = 1;

#ifdef DO_PARALLEL
	std::vector<Color> indecies(WIDTH);
	int temp = -1;
	std::generate(indecies.begin(),indecies.end(),[&temp](){return ++temp;});
	concurrency::array<const int, 1> indeciesAMP(WIDTH, indecies.begin(), indecies.end());
#endif
    
	for(unsigned int t = 0;t <= maxTime;++t)
	{
		bitmap_image bmp(WIDTH, HEIGHT);
		double max = 0;
        
		std::cout << "Raytracing " << t << std::endl;
#ifdef DO_PARALLEL

        for(int x = 0;x < WIDTH;++x)
        {
			concurrency::array<Color, 1> column(HEIGHT, image[x].begin(), image[x].end());

				concurrency::parallel_for_each(
				column.extent,
				[=,&column](concurrency::index<1> idx) restrict(amp)
				{
					column[idx] = renderPixel(x,indeciesAMP[idx],t);
				}
			);

            
                
            //if(max < color.r)
            //{
            //    max = color.r;
            //}
            //if(max < color.g)
            //{
            //    max = color.g;
            //}
            //if(max < color.b)
            //{
            //    max = color.b;
            //}
        }
#else
				for(int y = 0;y < HEIGHT;++y)
        {
            for(int x = 0;x < WIDTH;++x)
            {
                auto color = image[x][y] = renderPixel(x,y,t);
                
                if(max < color.r)
                {
                    max = color.r;
                }
                if(max < color.g)
                {
                    max = color.g;
                }
                if(max < color.b)
                {
                    max = color.b;
                }
            }
            if(y%10 == 0)
                std::cout << "Processing row " << y << std::endl;
        }
#endif


        
#ifdef ADD_HDR
        
        std::vector<int> histo(INT16_MAX);
        
        for(int y = 0;y < HEIGHT;++y)
        {
            for(int x = 0;x < WIDTH;++x)
            {
                auto scale = 1/max;
                image[x][y] = image[x][y]*scale;
                auto m = static_cast<int>(image[x][y].max()*INT16_MAX);
                histo[m]++;
                
            }
        }
        
//        histo[0] = 0;
        
        std::vector<int> cdf(INT16_MAX);
        
        for(unsigned int i=0; i<INT16_MAX; ++i)
        {
            cdf[i] = [&histo](int i)->int{
                int sum = 0;
                std::for_each(histo.begin(), histo.begin()+i+1, [&sum](int v){
                    sum += v;
                });
                return sum;
            }(i);
        }
        
        for(int y = 0;y < HEIGHT;++y)
        {
            for(int x = 0;x < WIDTH;++x)
            {
                auto color = image[x][y];
                auto cMin = color.min();
                auto cMax = color.max();
                
                //convert to hsv
                auto h = 0.0;
                auto s = cMax-cMin;
                if(s != 0)
                    s /= cMax;
                auto cMax16 = static_cast<int>(cMax*INT16_MAX);
                //apply cdf to val
                auto v = static_cast<double>(cdf[cMax16] - cdf[0])/static_cast<double>(NUMBER_OF_PIXELS - cdf[0]);
                if(cMax16 == 0)
                    v = 0;
                
                //set hue
                if(s != 0)
                {
                    color.r = (color.r - cMin)/s;
                    color.g = (color.g - cMin)/s;
                    color.b = (color.b - cMin)/s;
                    cMin = color.min();
                    cMax = color.max();
                    
                    if (cMax == color.r)
                    {
                        h = 0.0 + 60.0*(color.g - color.b);
                        if (h < 0.0) {
                            h += 360.0;
                        }
                    }
                    else if (cMax == color.g)
                    {
                        h = 120.0 + 60.0*(color.b - color.r);
                    }
                    else
                    {
                        h = 240.0 + 60.0*(color.r - color.g);
                    }
                }
                
                //convert back to rgb
                if( s == 0 ) {
                    // achromatic (grey)
                    color = Color(v,v,v);
                }
                else
                {
                    h /= 60.0;
                    int i = static_cast<int>(floor( h ));
                    auto f = h - i;
                    auto p = v * ( 1 - s );
                    auto q = v * ( 1 - s * f );
                    auto t = v * ( 1 - s * ( 1 - f ) );
                    switch( i ) {
                        case 0:
                            color = Color(v, t, p);
                            break;
                        case 1:
                            color = Color(q, v, p);
                            break;
                        case 2:
                            color = Color(p, v, t);
                            break;
                        case 3:
                            color = Color(p, q, v);
                            break;
                        case 4:
                            color = Color(t, p, v);
                            break;
                        default:
                            color = Color(v, p, q);
                            break;
                    }
                }
                
                bmp.set_pixel(x, HEIGHT-y-1, static_cast<unsigned char>(floor(color.r*255.0)), static_cast<unsigned char>(floor(color.g*255.0)), static_cast<unsigned char>(floor(color.b*255.0)));
            }
        }
#else
        
        for(int y = 0;y < HEIGHT;++y)
        {
            for(int x = 0;x < WIDTH;++x)
            {
                auto scale = 255.0/max;
                image[x][y] = image[x][y]*scale;
				bmp.set_pixel(x, HEIGHT-y-1, static_cast<unsigned char>(floor(image[x][y].r)), static_cast<unsigned char>(floor(image[x][y].g)), static_cast<unsigned char>(floor(image[x][y].b)));
            }
        }
#endif
        
        
        std::ostringstream file;
        file << "tmp/";
        for(unsigned int z = 0; z < 4-std::log10(t+1); ++z)
        {
            file << "0";
        }
        file << (t+1) << ".bmp";
        
        bmp.save_image(file.str());
	}
}


boost::optional<double> RayTracer::hitDistance(Ray r)
{
    return objects->hitDistance(r);
}

std::pair<boost::optional<double>,HitRecord> RayTracer::hit(Ray r)
{
    return objects->hit(r);
}
