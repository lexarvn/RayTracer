RayTracer
=========

Ray Tracing program for realistic renderings.

Supported effects: caustics, reflections, color bleed, shadows, indirect lighting, depth of field, anti-aliasing, specular jitter, and motion blur. 
Supported objects: triangles, spheres, and meshes defined by plain text ply files.
Supported materials: diffuse, specular, dielectric, emmissive, and compound marterials defined as ratios of other materials. 

Textures can be defined as bitmaps, colors, and soild noise.
Timelines can be defined for animation.
Simple HDR processing has also been implemented.

All programming done in c++11.
Scenes are defined in c++. Examples can be found in Main.cpp
Depends on optional from boost library.


Note: Bitmaps and ply files must be in the same working directory as the compiled program.
