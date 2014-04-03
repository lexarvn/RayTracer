#ifndef MESH_HPP
#define MESH_HPP
#include "Scene.hpp"
#include "MeshTriangle.hpp"
#include "Keyframed.hpp"
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <boost/optional.hpp>

class Mesh: public Scene
{
public:
	Mesh(std::string filename, std::shared_ptr<Material> m, Vector position, double scale);
private:
    void readFile(std::string filename, Vector position, double scale);
};

#endif