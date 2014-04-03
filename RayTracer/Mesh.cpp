#include "Mesh.hpp"

namespace
{
    Vector calcNormal(Vector a, Vector b, Vector c)
    {
        auto U = b - a;
        auto V = c - a;
        
        auto n = U.cross(V);
        
        return n;
    }
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
	std::vector<Vector> vertices;
	std::vector<Vector> normals;
	std::vector<Vector> faces;
    
    
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
				normals.reserve(numVertices);
                vertices.reserve(numVertices);
                std::fill(normals.begin(), normals.end(), Vector());
			}
			else if(chunk == "face")
				file >> numFaces;
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
        
        temp *= scale;
        temp += position;
        
		vertices.push_back(temp);
        
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
		
        Vector temp(x,y,z);
        
		faces.push_back(temp);
        
        auto n = calcNormal(vertices[x],vertices[y],vertices[z]);
        if(!(n == Vector()))
        {
            n.normalize();
            normals[x] += n;
            normals[y] += n;
            normals[z] += n;
        }
	}
    
    for(auto &n:normals)
    {
        if(!(n == Vector()))
            n.normalize();
    }
    
    for(auto p:faces)
    {
		shapes.push_back(std::shared_ptr<MeshTriangle>(new MeshTriangle(vertices[(int)p.x],vertices[(int)p.y],vertices[(int)p.z],normals[(int)p.x],normals[(int)p.y],normals[(int)p.z],m)));
    }
}
