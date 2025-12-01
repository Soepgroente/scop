#include "VulkanObject.hpp"

#include <iostream>
#include <map>
#include <functional>
#include <sstream>
#include <fstream>

namespace ve {

static float getNextFloat(std::istringstream& stream)
{
	float nextFloat;

	stream >> nextFloat;
	if (stream.fail() == true)
	{
		throw std::runtime_error("Failed to parse float value");
	}
	return nextFloat;
}

static glm::vec3	parseColor(std::istringstream& colorData)
{
	glm::vec3	color;

	color.r = getNextFloat(colorData);
	color.g = getNextFloat(colorData);
	color.b = getNextFloat(colorData);
	if (colorData.eof() == false)
	{
		throw std::runtime_error("Invalid color format");
	}
	return color;
}

static void	setModelName(ObjInfo& obj, std::istringstream& name)
{
	std::string modelName;

	name >> modelName;
	if (name.eof() == false || modelName.empty())
	{
		throw std::runtime_error("Invalid VulkanObject name format");
	}
	obj.name = modelName;
}

static void	addVertex(ObjInfo& obj, std::istringstream& vertexData)
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
	obj.vertices.push_back(glm::vec3(x, y, z));
}

static void	addTextureCoord(ObjInfo& obj, std::istringstream& texCoordData)
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
	obj.textureCoords.push_back(glm::vec3(u, v, w));
}

static void	addSurfaceNormal(ObjInfo& obj, std::istringstream& normalData)
{
	float	x, y, z;

	x = getNextFloat(normalData);
	y = getNextFloat(normalData);
	z = getNextFloat(normalData);
	if (normalData.eof() == false)
	{
		throw std::runtime_error("Invalid surface normal format");
	}
	obj.normals.push_back(glm::vec3(x, y, z));
}

static void	addFace(ObjInfo& obj, std::istringstream& faceData)
{
	std::string	vertex;
	ObjComponent&	objComp = obj.components.back();

	if (objComp.matName.empty() == true)
	{
		objComp.matName = "default";
	}

	while (faceData.eof() == false)
	{
		faceData >> vertex;
		size_t		firstSlash = vertex.find('/');

		objComp.faceIndices.push_back(static_cast<uint32_t>
			(std::stoi(vertex.substr(0, firstSlash))) - 1);
		if (firstSlash != std::string::npos)
		{
			size_t	secondSlash = vertex.find('/', firstSlash + 1);
			if (secondSlash > firstSlash + 1)
			{
				objComp.textureIndices.push_back(static_cast<uint32_t>
					(std::stoi(vertex.substr(firstSlash + 1, secondSlash - firstSlash - 1))) - 1);
			}
			if (secondSlash != std::string::npos)
			{
				objComp.normalIndices.push_back(static_cast<uint32_t>
					(std::stoi(vertex.substr(secondSlash + 1))) - 1);
			}
		}
	}
}

static void	parseMaterial(ObjInfo& obj, std::istringstream& materialData)
{
	std::string	materialFileName;

	materialData >> materialFileName;
	if (materialData.eof() == false || materialFileName.empty())
	{
		throw std::runtime_error("Invalid material file name format");
	}
	std::string		line;
	std::fstream	file("./models/" + materialFileName);
	std::string			currentMat;
	std::map<std::string, Material>&	mats = obj.materials;
	
	if (file.is_open() == false)
	{
		throw std::runtime_error("Could not open material file: " + line);
	}
	materialData >> line;
	std::map<std::string, std::function<void (std::istringstream&)>> materialMethods =
	{
		{"newmtl", { [&](std::istringstream& matData)
			{				
				matData >> line;
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid material name format");
				}
				if (mats.find(line) == mats.end())
				{
					Material newMat{};
					newMat.name = line;
					mats.insert({line, newMat});
					currentMat = line;
				}
				else
				{
					throw std::runtime_error("Duplicate material name: " + line);
				}
			} }
		},
		{"Ka", { [&](std::istringstream& matData)
			{
				mats[currentMat].ambientClr = parseColor(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid ambient color format");
				}
			} }
		},
		{"Kd", { [&](std::istringstream& matData)
			{
				mats[currentMat].diffuseClr = parseColor(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid diffuse color format");
				}
			} }
		},
		{"Ks", { [&](std::istringstream& matData)
			{
				mats[currentMat].specularClr = parseColor(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid specular color format");
				}
			} }
		},
		{"Ns", { [&](std::istringstream& matData)
			{
				mats[currentMat].shininess = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid shininess format");
				}
			} }
		},
		{"d", { [&](std::istringstream& matData)
			{
				mats[currentMat].opacity = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid opacity format");
				}
			} }
		},
		{"illum", { [&](std::istringstream& matData)
			{
				mats[currentMat].illuminationModel = getNextFloat(matData);
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid illumination model format");
				}
			} }
		},
		{"s", { [&](std::istringstream& matData)
			{
				std::string	smoothShading;
				
				matData >> smoothShading;
				if (matData.eof() == false)
				{
					throw std::runtime_error("Invalid smooth shading format");
				}
				if (smoothShading == "1" || smoothShading == "on")
				{
					mats[currentMat].smoothShading = true;
				}
				else
				{
					mats[currentMat].smoothShading = false;
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
			file.close();
			throw std::runtime_error("Error parsing material file " + line);
		}
	}
	file.close();
}

static void	useMaterial(ObjInfo& obj, std::istringstream& matData)
{
	std::string	materialName;

	matData >> materialName;
	if (matData.eof() == false || materialName.empty())
	{
		throw std::runtime_error("Invalid usemtl format");
	}
	if (obj.components.empty() == true ||
		obj.components.back().matName != materialName)
	{
		obj.components.push_back(ObjComponent{});
		obj.components.back().matName = materialName;
	}
}

static void	nullFunc(ObjInfo& obj, std::istringstream& data)
{
	/*	Not implemented	*/
	(void)obj;
	(void)data;
}

/*	Elements that can occur in .obj file:

	v - Vertices (3-4 floats)
	vt - Texture coordinates (1-3 floats)
	vn - Vertex normals (3 floats)
	f - Faces (vertex/texture/normal indices)

	o - VulkanObject names
	s - Smoothing groups
	usemtl - Material usage
	mtllib - Material library references
	g - Group names	(skipped)
	l - Lines, curves, surfaces (skipped)
*/

std::vector<ObjInfo>	parseOBJFile(const std::string& objFilePath)
{
	std::fstream	file(objFilePath);
	int lineCount = 0;

	if (file.is_open() == false)
	{
		std::cerr << "Error: Could not open OBJ file: " << objFilePath << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::string	line;
	std::map<std::string, std::function<void (ObjInfo&, std::istringstream&)>> parseMethods =
	{
		{"o", { &setModelName } },
		{"v", { &addVertex } },
		{"vt", { &addTextureCoord } },
		{"vn", { &addSurfaceNormal } },
		{"f", { &addFace } },
		{"usemtl", { &useMaterial } },
		{"mtllib", { &parseMaterial } },
		{"g", { &nullFunc } },
		{"l", { &nullFunc } },
	};
	std::vector<ObjInfo>	objs;
	ObjInfo	currentObj{};
	currentObj.components.push_back(ObjComponent{});

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
		if (word == "o" && currentObj.vertices.size() > 0)
		{
			objs.emplace_back(std::move(currentObj));
			currentObj = ObjInfo{};
		}
		try
		{
			parseMethods[word](currentObj, lineStream);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			std::cerr << "Error parsing OBJ file: " << objFilePath << " at line " << lineCount << ": " << line << std::endl;
			file.close();
			std::exit(EXIT_FAILURE);
		}
	}
	objs.emplace_back(std::move(currentObj));
	for (ObjInfo& obj : objs)
	{
		for (ObjComponent& component : obj.components)
		{
			if ((component.faceIndices.empty() != true && *std::max_element(component.faceIndices.begin(), component.faceIndices.end()) >= obj.vertices.size()) ||
				(component.textureIndices.empty() != true && *std::max_element(component.textureIndices.begin(), component.textureIndices.end()) >= obj.textureCoords.size()) ||
				(component.normalIndices.empty() != true && *std::max_element(component.normalIndices.begin(), component.normalIndices.end()) >= obj.normals.size()))
			{
				std::cerr << "Error: index out of bounds " << obj.name << std::endl;
				file.close();
				std::exit(EXIT_FAILURE);
			}
		}
	}
	file.close();
	return objs;
}

std::ostream&	operator<<(std::ostream& os, const ObjInfo& obj)
{
	os << "Object Name: " << obj.name << "\n";
	os << "Vertices: " << obj.vertices.size() << "\n";
	os << "Texture Coords: " << obj.textureCoords.size() << "\n";
	os << "Normals: " << obj.normals.size() << "\n";
	os << "Components: " << obj.components.size() << "\n";
	for (const auto& matPair : obj.materials)
	{
		os << "Material Name: " << matPair.first << "\n";
		os << "Listed name: " << matPair.second.name << "\n";
	}
	return os;
}

} // namespace ve
