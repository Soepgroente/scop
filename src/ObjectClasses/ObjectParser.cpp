#include "ObjectParser.hpp"

static float getNextFloat(std::istringstream& stream)
{
	float nextFloat;

	stream >> nextFloat;
	if (stream.fail() == true)
	{
		throw std::runtime_error("Invalid float format");
	}
	return nextFloat;
}

static void	setObjectName(Object& obj, std::istringstream& name)
{
	std::string objectName;

	name >> objectName;
	if (name.eof() == false || objectName.empty())
	{
		throw std::runtime_error("Invalid object name format");
	}
	obj.name = objectName;
}

static void	addVertex(Object& obj, std::istringstream& vertexData)
{
	float	x, y, z, w = 1.0f;

	x = getNextFloat(vertexData);
	y = getNextFloat(vertexData);
	z = getNextFloat(vertexData);
	if (vertexData.eof() == false)
	{
		vertexData >> w;
	}
	if (vertexData.eof() == false || vertexData.fail() == true)
	{
		throw std::runtime_error("Invalid vertex format");
	}
	obj.mesh.vertices.emplace_back(Vec4(w, x, y, z));
}

static void	addTextureCoord(Object& obj, std::istringstream& texCoordData)
{
	float	u, v = 0.0f, w = 0.0f;

	u = getNextFloat(texCoordData);
	if (texCoordData.eof() == false)
	{
		v = getNextFloat(texCoordData);
	}
	if (texCoordData.eof() == false)
	{
		w = getNextFloat(texCoordData);
	}
	if (texCoordData.eof() == false)
	{
		throw std::runtime_error("Invalid texture coordinate format");
	}
	obj.mesh.textureCoords.emplace_back(Vec3(u, v, w));
}

static void	addSurfaceNormal(Object& obj, std::istringstream& normalData)
{
	float	x, y, z;

	x = getNextFloat(normalData);
	y = getNextFloat(normalData);
	z = getNextFloat(normalData);
	if (normalData.eof() == false)
	{
		throw std::runtime_error("Invalid surface normal format");
	}
	obj.mesh.normals.emplace_back(Vec3(x, y, z));
}

static void	addFace(Object& obj, std::istringstream& faceData)
{
	Faces	face;

	for (int i = 0; i < 3; i++)
	{
		faceData >> face.faces[i];
		if (faceData.fail() == true)
		{
			throw std::runtime_error("Invalid face format");
		}
	}
	obj.mesh.faces.emplace_back(face);
}

static void	material(Object& obj, std::istringstream& materialData)
{
	;
}

/*	Elements that can occur in .obj file:

	v - Vertices (3-4 floats)
	vt - Texture coordinates (1-3 floats)
	vn - Vertex normals (3 floats)
	f - Faces (vertex/texture/normal indices)

	o - Object names
	g - Group names
	s - Smoothing groups
	usemtl - Material usage
	mtllib - Material library references
	l - Lines, curves, surfaces (often skipped in basic parsers) */

void	ObjectParser::addObjectsFromFile(const std::string& objFilePath, Scene& scene)
{
	std::fstream	file(objFilePath);
	int lineCount = 0;

	if (file.is_open() == false)
	{
		throw std::runtime_error("Failed to open OBJ file: " + objFilePath);
	}
	std::string	materialFile = objFilePath.substr(0, objFilePath.find_last_of('.')) + ".mtl";
	std::string	line;
	std::map<std::string, std::function<void (Object&, std::istringstream&)>> parseMethods =
	{
		{"o", { &setObjectName } },
		{"v", { &addVertex } },
		{"vt", { &addTextureCoord } },
		{"vn", { &addSurfaceNormal } },
		{"f", { &addFace } },
		{"usemtl", { &material } },
		{"mtllib", { &material } },
		{"s", { &material } },
	};

	Object obj;

	while (file.eof() == false)
	{
		std::getline(file, line);
		lineCount++;
		
		if (line[0] == '\n' || line[0] == '#')
		{
			continue;
		}
		std::istringstream	lineStream(line);
		std::string			word;
		
		lineStream >> word;
		if (word == "o")
		{
			if (obj.name.empty() == false)
			{
				scene.addObject(obj);
				obj = Object();
			}
		}
		try
		{
			parseMethods[word](obj, lineStream);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			throw std::runtime_error("Error parsing " + objFilePath + " at line " + std::to_string(lineCount));
		}
	}
	scene.addObject(obj);
}