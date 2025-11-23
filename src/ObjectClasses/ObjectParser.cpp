#include "ObjectParser.hpp"
#include <map>

namespace ve {

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

static void	setVulkanObjectName(VulkanObject& obj, std::istringstream& name)
{
	std::string VulkanObjectName;

	name >> VulkanObjectName;
	if (name.eof() == false || VulkanObjectName.empty())
	{
		throw std::runtime_error("Invalid VulkanObject name format");
	}
	obj.setName(VulkanObjectName);
}

static void	addVertex(VulkanObject& obj, std::istringstream& vertexData)
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
	(void)z;
	(void)w;
	obj.addVertex({{x, y}, {1.0f, 1.0f, 1.0f}});
}

static void	addTextureCoord(VulkanObject& obj, std::istringstream& texCoordData)
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
	obj.addTextureCoord(glm::vec3(u, v, w));
}

static void	addSurfaceNormal(VulkanObject& obj, std::istringstream& normalData)
{
	float	x, y, z;

	x = getNextFloat(normalData);
	y = getNextFloat(normalData);
	z = getNextFloat(normalData);
	if (normalData.eof() == false)
	{
		throw std::runtime_error("Invalid surface normal format");
	}
	obj.addSurfaceNormal(glm::vec3(x, y, z));
}

static void	addFace(VulkanObject& obj, std::istringstream& faceData)
{
	std::vector<int>	faceIndices;
	std::string			vertexDef;

	while (faceData.eof() == false)
	{
		faceData >> vertexDef;
		if (faceData.fail() == true)
		{
			throw std::runtime_error("Invalid face format");
		}
		std::istringstream	vertexDefStream(vertexDef);
		std::string			indexStr;
		int					vertexIndex;

		std::getline(vertexDefStream, indexStr, '/');
		vertexIndex = std::stoi(indexStr);
		if (vertexIndex < 0)
		{
			throw std::runtime_error("Negative vertex indices are not supported");
		}
		faceIndices.push_back(vertexIndex - 1);
	}
	if (faceIndices.size() < 3)
	{
		throw std::runtime_error("Face must have at least 3 vertices");
	}
	obj.addFace(faceIndices);
}

static void	material(VulkanObject& obj, std::istringstream& materialData)
{
	VulkanObject::Material	mat;
	std::string			line;

	materialData >> line;

	std::fstream		file("./examples/" + line);

	if (file.is_open() == false)
	{
		throw std::runtime_error("Could not open material file: " + line);
	}
	std::map<std::string, std::function<void (std::istringstream&)>> materialMethods =
	{
		{"newmtl", { [&](std::istringstream& matData)
			{
				matData >> mat.name;
				if (matData.eof() == false || mat.name.empty())
				{
					throw std::runtime_error("Invalid material name format");
				}
				mat.exists = true;
			} }
		},
		{"Ka", { [&](std::istringstream& matData)
			{
				mat.ambientClr.r = getNextFloat(matData);
				mat.ambientClr.g = getNextFloat(matData);
				mat.ambientClr.b = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid ambient color format");
				}
			} }
		},
		{"Kd", { [&](std::istringstream& matData)
			{
				mat.diffuseClr.r = getNextFloat(matData);
				mat.diffuseClr.g = getNextFloat(matData);
				mat.diffuseClr.b = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid diffuse color format");
				}
			} }
		},
		{"Ks", { [&](std::istringstream& matData)
			{
				mat.specularClr.r = getNextFloat(matData);
				mat.specularClr.g = getNextFloat(matData);
				mat.specularClr.b = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid specular color format");
				}
			} }
		},
		{"Ns", { [&](std::istringstream& matData)
			{
				mat.specularExp = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid specular exponent format");
				}
			} }
		},
		{"d", { [&](std::istringstream& matData)
			{
				mat.transparency = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid transparency format");
				}
			} }
		},
		{"illum", { [&](std::istringstream& matData)
			{
				mat.illumModel = static_cast<int>(getNextFloat(matData));
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid illumination model format");
				}
			} }
		},
	};
	while (file.eof() == false)
	{
		std::getline(file, line);
		std::istringstream	lineStream(line);
		std::string			word;

		lineStream >> word;
		if (lineStream.eof() == true || word[0] == '#')
		{
			continue;
		}
		try
		{
			materialMethods.at(word)(lineStream);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			throw std::runtime_error("Error parsing material file " + line);
		}
	}
	obj.setMaterial(std::move(mat));
}

/*	Elements that can occur in .obj file:

	v - Vertices (3-4 floats)
	vt - Texture coordinates (1-3 floats)
	vn - Vertex normals (3 floats)
	f - Faces (vertex/texture/normal indices)

	o - VulkanObject names
	g - Group names
	s - Smoothing groups
	usemtl - Material usage
	mtllib - Material library references
	l - Lines, curves, surfaces (often skipped in basic parsers) */

std::vector<VulkanObject>	ObjectParser::parseOBJFile(const std::string& objFilePath)
{
	std::fstream	file(objFilePath);
	int lineCount = 0;

	if (file.is_open() == false)
	{
		std::cerr << "Error: Could not open OBJ file: " << objFilePath << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string	materialFile = objFilePath.substr(0, objFilePath.find_last_of('.')) + ".mtl";
	std::string	line;
	std::map<std::string, std::function<void (VulkanObject&, std::istringstream&)>> parseMethods =
	{
		{"o", { &setVulkanObjectName } },
		{"v", { &addVertex } },
		{"vt", { &addTextureCoord } },
		{"vn", { &addSurfaceNormal } },
		{"f", { &addFace } },
		{"usemtl", { &material } },
		{"mtllib", { &material } },
		{"s", { &material } },
	};

	std::vector<VulkanObject> objs;
	VulkanObject currentObj = VulkanObject::createVulkanObject();

	while (file.eof() == false)
	{
		std::getline(file, line);
		lineCount++;
		
		if (line.size() == 0 || line[0] == '\n' || line[0] == '#')
		{
			continue;
		}
		std::istringstream	lineStream(line);
		std::string			word;
		
		lineStream >> word;
		if (word == "o")
		{
			if (currentObj.getName().empty() == false)
			{
				objs.emplace_back(std::move(currentObj));
				currentObj = VulkanObject::createVulkanObject();
			}
		}
		try
		{
			parseMethods[word](currentObj, lineStream);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			throw std::runtime_error("Error parsing " + objFilePath + " at line " + std::to_string(lineCount));
		}
	}
	objs.emplace_back(std::move(currentObj));
	return objs;
}

} // namespace ve