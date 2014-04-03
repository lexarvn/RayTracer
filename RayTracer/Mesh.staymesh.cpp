#include "Mesh.staymesh.hpp"

namespace
{
    Vector calcNormal(Vector a, Vector b, Vector c)
    {
        auto U = b - a;
        U.normalize();
        auto V = c - a;
        V.normalize();
        
        auto n = U.cross(V);
        n.normalize();
        
        return n;
    }
}

Vector Mesh::getPoint(int i)
{
    return meshPoints[i];
}

Vector Mesh::getNormal(int i)
{
    return meshNormals[i];
}

Mesh::Mesh(std::string fName, std::shared_ptr<Material> mat, Vector position, double scale)
{
    m = mat;
    std::cout << "Loading " << fName << std::endl;
    readFile(fName,position,scale);
    std::cout << "Building hierarchy for " << fName << std::endl;
    buildHierarchy();
}


void Mesh::readFile(std::string fName, Vector position, double scale)
{
	std::vector<std::tuple<int,int,int>> faces;
    
    
	int numVertices;
	int numFaces;
	bool hasConfidence = false;
	bool hasIntensity = false;
    
	double confidence;
	double intensity;
	std::string line;
	std::string chunk;
    
	std::ifstream file(fName, std::ios::in);
	std::getline(file, line);
    
	if(!file)
	{
        throw std::runtime_error("Mesh file doesn't exsist.");
	}

	//Read header
	while(!file.eof())
	{
		std::getline(file, line);
        
		file >> chunk;
        
		if(chunk == "element")
		{
			file >> chunk;
			if(chunk == "vertex")
			{
				file >> numVertices;
                meshNormals = std::vector<Vector>(numVertices);
                meshPoints = std::vector<Vector>();
                meshPoints.reserve(numVertices);
			}
			else if(chunk == "face")
            {
				file >> numFaces;
                faces.reserve(numFaces);
            }
		}
        
		
        
		if(chunk == "property")
		{
			file >> chunk;//Read in data type
			file >> chunk;//Read in property
            
			if(chunk == "confidence")
				hasConfidence = true;
			if(chunk == "intensity")
				hasIntensity = true;
		}
        
		if(chunk == "end_header")
			break;
        
	}
    
	//Store Points
	for(int i = 0; i < numVertices; ++i)
	{
		Vector temp;
        
		file >> temp.x;
		file >> temp.y;
		file >> temp.z;
        
		meshPoints.push_back(temp);
        
		if(hasConfidence)
			file >> confidence;
		if(hasIntensity)
			file >> intensity;
	}
    
	//Store faces
	for(int i = 0; i < numFaces; ++i)
	{
        //junk
		file >> chunk;
        
        int x,y,z;
		file >> x;
		file >> y;
		file >> z;
        
		faces.push_back(std::make_tuple(x,y,z));
        
        auto xv = meshPoints[x];
        auto yv = meshPoints[y];
        auto zv = meshPoints[z];

        auto n = calcNormal(xv,yv,zv);
        meshNormals[x] += n;
        meshNormals[y] += n;
        meshNormals[z] += n;
	}
    
    //normalize normals
    for(auto &n:meshNormals)
    {
        if(!(n == Vector()))
            n.normalize();
    }
    
    for(auto &p:meshPoints)
    {
        p *= scale;
        p += position;
    }
    
    for(auto &f:faces)
    {
		shapes.push_back(std::shared_ptr<MeshTriangle>(new MeshTriangle(std::get<0>(f),std::get<1>(f),std::get<2>(f),std::make_shared<Mesh>(*this),m)));
    }
}
