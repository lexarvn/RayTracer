#include "RayTracer.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Mesh.hpp"
#include "Instance.hpp"
#include "Matrix.hpp"
#include "Eye.hpp"
#include <memory>

void defaultScene(); //very random
void dragonAndBunny();
void indirectLighting();
void caustics();

int main()
{
    defaultScene();
	return 0;
}

void defaultScene(){
	const int DISTANCE = 500;
	const int WIDTH = 640;
	const int HEIGHT = 480;
	auto s = RayTracer::getInstance();
    
    s->setEye(Eye(Vector(320,240,500), Vector(320,240,0), Vector(0,1,0), DISTANCE, WIDTH, HEIGHT, 3, WIDTH, HEIGHT));
    s->maxTime = 10;
    s->maxRays = 125;
    
    auto light = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(1,0,0));
    s->addLight(std::make_shared<Sphere>(Vector(0,100,600),300,light));
    s->addLight(std::make_shared<Sphere>(Vector(0,600,600),300,light));
    s->addLight(std::make_shared<Sphere>(Vector(0,-400,600),300,light));
    
    auto light2 = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(0,1,0));
    s->addLight(std::make_shared<Sphere>(Vector(300,200,810),300,light2));
    s->addLight(std::make_shared<Sphere>(Vector(300,700,810),300,light2));
    s->addLight(std::make_shared<Sphere>(Vector(300,-300,810),300,light2));
    
    auto light3 = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(0,0,1));
    s->addLight(std::make_shared<Sphere>(Vector(800,1000,300),300,light3));
    s->addLight(std::make_shared<Sphere>(Vector(800,500,300),300,light3));
    s->addLight(std::make_shared<Sphere>(Vector(800,0,300),300,light3));
    
	Keyframed<double> r;
	r.addFrame(0,100);
	r.addFrame(5,50);
	r.addFrame(10,150);
	Keyframed<Vector> v;
	v.addFrame(0,Vector(500,100,-55));
	v.addFrame(10,Vector(400,150,-85));
    
	auto m1 = std::make_shared<SpecularMaterial>(std::make_shared<SolidColor>(.95,.95,0),.3);
	auto m2 = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,.95,.95));
	auto m3 = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,0,.95));
	auto m4 = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,.95,0));
	auto m5 = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,0,0));
	auto m6 = std::make_shared<CheckeredMaterial>(m5,std::make_shared<CheckeredMaterial>(m3,m4,10),20);
	s->addObject(std::make_shared<Sphere>(v,r,m1));
	auto m7 = std::make_shared<DiffuseMaterial>(std::make_shared<ColorMap>("earth.bmp"));
	auto m8 = std::make_shared<DielectricMaterial>(std::make_shared<SolidColor>(.95,.95,.95),1.3,.05);
	s->addObject(std::make_shared<Sphere>(Vector(320, 240, 350),600,m2));
    
    Keyframed<Vector> t;
	t.addFrame(0,Vector(200,300,400));
	t.addFrame(10,Vector(150,300,100));
    
	s->addObject(std::shared_ptr<Instance>(new Instance(t,0,0,0,Vector(1,1,1),std::make_shared<Sphere>(Vector(),100,m6))));
    
    Keyframed<double> r2;
	r2.addFrame(0,0);
	r2.addFrame(10,360);
    
	s->addObject(std::shared_ptr<Instance>(new Instance(Vector(400,350,-100),0,r2,0,Vector(1,1,1),std::make_shared<Sphere>(Vector(),100,m7))));
	s->addObject(std::shared_ptr<Instance>(new Instance(Vector(320, 240, 100),0,0,0,Vector(1,1,1),std::make_shared<Sphere>(Vector(),100,m8))));
    
    Keyframed<Vector> a;
	a.addFrame(0,Vector(300,200,-5));
	a.addFrame(5,Vector(250,220,-5));
    Keyframed<Vector> b;
	b.addFrame(0,Vector(350,200,-5));
	b.addFrame(7,Vector(325,190,-95));
    Keyframed<Vector> c;
	c.addFrame(0,Vector(325,260,-35));
	c.addFrame(10,Vector(230,290,-5));
	s->addObject(std::make_shared<Triangle>(a,b,c,m3));
    
    auto bunny = std::make_shared<Mesh>("bun_zipper_res4.ply",std::make_shared<MarbleMaterial>(std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,.95,.95)),15),Vector(0,0,0),75);
    
    Keyframed<double> ry,rxz;
    rxz.addFrame(0, 0);
    rxz.addFrame(10, 5);
    ry.addFrame(0, 0);
    ry.addFrame(10, 180);
    
    s->addObject(std::shared_ptr<Instance>(new Instance(Vector(200,20,0),rxz,ry,rxz,Vector(13,13,13), bunny)));
    
    s->renderScene();
}

void dragonAndBunny(){
	const int DISTANCE = 500;
	const int WIDTH = 640;
	const int HEIGHT = 480;
	auto s = RayTracer::getInstance();
    
    s->setEye(Eye(Vector(0,0,0), Vector(0,0,1), Vector(0,1,0), DISTANCE, WIDTH, HEIGHT, 5, 640, 480));
    s->maxTime = 60;
    s->maxRays = 125;
    
    auto light = std::make_shared<EmissiveMaterial>(std::make_shared<ColorMap>("sun.bmp"));
//	auto light = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(1,1,1));
    s->addLight(std::make_shared<Sphere>(Vector(80,75,100),50,light));

	auto mDragon = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(0.9,0,0.35));
	auto mBunny  = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,.95,.95));
	auto mEarth  = std::make_shared<DiffuseMaterial>(std::make_shared<ColorMap>("earth.bmp"));
	auto mSky    = std::make_shared<DiffuseMaterial>(std::make_shared<ColorMap>("sky.bmp"));

	s->addObject(std::make_shared<Sphere>(Vector(0, 0, 500),700,mSky));
	auto earth = std::make_shared<Sphere>(Vector(0, 0, 0),120,mEarth);
    s->addObject(std::shared_ptr<Instance>(new Instance(Vector(0,0,500),0,170,0,Vector(1,1,1), earth)));

	Keyframed<double> rzBunny,rzDragon;
    rzBunny.addFrame(0, 0);
    rzBunny.addFrame(60, 360);
    rzDragon.addFrame(0, -50);
    rzDragon.addFrame(60, 310);

	auto bunny = std::make_shared<Mesh>("bun_zipper.ply",mBunny,Vector(0,100,0),500);
    s->addObject(std::shared_ptr<Instance>(new Instance(Vector(0,0,500),5,180,rzBunny,Vector(1,1,1), bunny)));
	
	auto dragon = std::make_shared<Mesh>("dragon_vrip.ply",mDragon,Vector(0,75,0),700);
    s->addObject(std::shared_ptr<Instance>(new Instance(Vector(0,0,500),-20,180,rzDragon,Vector(1,1,1), dragon)));
    
    s->renderScene();
}

void indirectLighting()
{
	const int DISTANCE = 500;
	const int WIDTH = 640;
	const int HEIGHT = 480;
	auto s = RayTracer::getInstance();
    
    s->setEye(Eye(Vector(320,240,10), Vector(320,240,100), Vector(0,1,0), DISTANCE, WIDTH, HEIGHT, 5, 640, 480));
    s->maxTime = 0;
    s->maxRays = 1000;
    
    auto leftBottom  = Vector( 20, 20,  0);
    auto leftMid     = Vector( 20,240,  0);
    auto leftTop     = Vector( 20,460,  0);
    auto rightBottom = Vector(620, 20,  0);
    auto rightMid    = Vector(620,240,  0);
    auto rightTop    = Vector(620,460,  0);
    auto far = Vector(0, 0, 900);
    auto m1 = Vector(0, 0, 610);
    auto m2 = Vector(0, 0, 600);

	auto redWall  = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95,  0,.15));
	auto blueWall = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.15,  0,.95));
	auto whiteWall= std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.99,.99,.99));
    
	s->addObject(std::make_shared<Triangle>(leftBottom, leftTop,     leftBottom+far, redWall));
	s->addObject(std::make_shared<Triangle>(leftTop,    leftTop+far, leftBottom+far, redWall));
	s->addObject(std::make_shared<Triangle>(rightBottom+far, rightTop+far, rightBottom, redWall));
	s->addObject(std::make_shared<Triangle>(rightTop+far,    rightTop,     rightBottom, redWall));
    
	s->addObject(std::make_shared<Triangle>(leftBottom+far, leftTop+far,  rightBottom+far, blueWall));
	s->addObject(std::make_shared<Triangle>(leftTop+far,    rightTop+far, rightBottom+far, blueWall));
	s->addObject(std::make_shared<Triangle>(rightBottom, rightTop, leftBottom, blueWall));
	s->addObject(std::make_shared<Triangle>(rightTop,    leftTop,  leftBottom, blueWall));
    
    s->addObject(std::make_shared<Triangle>(rightBottom, leftBottom,     rightBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftBottom,  leftBottom+far, rightBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftTop,  rightTop,     leftTop+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(rightTop, rightTop+far, leftTop+far, whiteWall));
    
	s->addObject(std::make_shared<Triangle>(leftBottom+m1, leftMid+m1,  rightBottom+m1, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftMid+m1,    rightMid+m1, rightBottom+m1, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftMid+m1, leftMid+m2,  rightMid+m1, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftMid+m2, rightMid+m2, rightMid+m1, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftBottom+m2, leftMid+m2,  rightBottom+m2, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftMid+m2,    rightMid+m2, rightBottom+m2, whiteWall));
    
    auto lightOffset = Vector(0,10,0);
    
    leftMid  -= lightOffset;
    rightMid -= lightOffset;
    
    auto light = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(1,1,1));
	s->addLight(std::make_shared<Triangle>(leftMid+far, leftMid+m1,  rightMid+far, light));
	s->addLight(std::make_shared<Triangle>(leftMid+m1,  rightMid+m1, rightMid+far, light));
    
	auto mBunny = std::make_shared<MarbleMaterial>(std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.99,.99,.99)),15);
	auto bunny = std::make_shared<Mesh>("bun_zipper_res4.ply",mBunny,Vector(0,0,0),75);
    s->addObject(std::shared_ptr<Instance>(new Instance(Vector(320,0,500),0,180,0,Vector(17,17,17), bunny)));
    
    s->renderScene();
}

void caustics()
{
	const int DISTANCE = 500;
	const int WIDTH = 640;
	const int HEIGHT = 480;
	auto s = RayTracer::getInstance();
    
    s->setEye(Eye(Vector(320,240,10), Vector(320,240,100), Vector(0,1,0), DISTANCE, WIDTH, HEIGHT, 5, 640, 480));
    s->maxTime = 0;
    s->maxRays = 250;
    
    auto light = std::make_shared<EmissiveMaterial>(std::make_shared<SolidColor>(1,1,1));
	s->addLight(std::make_shared<Sphere>(Vector(-620,980,900),500, light));
    
    auto leftBottom  = Vector(-620, 20,  0);
    auto leftTop     = Vector(-620,980,  0);
    auto rightBottom = Vector( 660, 20,  0);
    auto rightTop    = Vector( 660,980,  0);
    auto far = Vector(0,0,1200);
    
	auto redWall  = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.95, .2, .2));
	auto blueWall = std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>( .2, .2,.95));
	auto whiteWall= std::make_shared<DiffuseMaterial>(std::make_shared<SolidColor>(.99,.99,.99));
    
	s->addObject(std::make_shared<Triangle>(leftBottom, leftTop,     leftBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftTop,    leftTop+far, leftBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(rightBottom+far, rightTop+far, rightBottom, blueWall));
	s->addObject(std::make_shared<Triangle>(rightTop+far,    rightTop,     rightBottom, blueWall));
    
	s->addObject(std::make_shared<Triangle>(leftBottom+far, leftTop+far,  rightBottom+far, redWall));
	s->addObject(std::make_shared<Triangle>(leftTop+far,    rightTop+far, rightBottom+far, redWall));
	s->addObject(std::make_shared<Triangle>(rightBottom, rightTop, leftBottom, whiteWall));
	s->addObject(std::make_shared<Triangle>(rightTop,    leftTop,  leftBottom, whiteWall));
    
    s->addObject(std::make_shared<Triangle>(rightBottom, leftBottom,     rightBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftBottom,  leftBottom+far, rightBottom+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(leftTop,  rightTop,     leftTop+far, whiteWall));
	s->addObject(std::make_shared<Triangle>(rightTop, rightTop+far, leftTop+far, whiteWall));
    
	auto dielectric = std::make_shared<DielectricMaterial>(std::make_shared<SolidColor>(.95,.95,.95),1.7,0);
    s->addObject(std::make_shared<Sphere>(Vector(170,170,800),200, dielectric));
    
    s->renderScene();
}
