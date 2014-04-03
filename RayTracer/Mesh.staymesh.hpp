#ifndef MESH_HPP
#define MESH_HPP
#include "Scene.hpp"
#include "MeshTriangle.staymesh.hpp"
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
    Vector getPoint(int);
    Vector getNormal(int);
private:
    void readFile(std::string filename, Vector position, double scale);
    std::vector<Vector> meshPoints;
    std::vector<Vector> meshNormals;
};

#endif