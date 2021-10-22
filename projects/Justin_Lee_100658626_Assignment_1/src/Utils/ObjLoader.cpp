#include "ObjLoader.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

// Borrowed from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
#pragma region String Trimming

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

#pragma endregion 

VertexArrayObject::Sptr ObjLoader::LoadFromFile(const std::string& filename)
{
	// Open our file in binary mode
	std::ifstream file;
	file.open(filename, std::ios::binary);

	// If our file fails to open, we will throw an error
	if (!file) {
		throw std::runtime_error("Failed to open file");
	}

	std::string line;

	// TODO: Load data from file
	std::vector<glm::vec3> positions;
	std::vector<glm::ivec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	glm::vec3 vecData;
	glm::ivec3 vertexIndices;
	glm::vec2 uvsData;
	glm::vec3 normalsData;

	// Read and process the entire file
	while (file.peek() != EOF) {
		// Read in the first part of the line (ex: f, v, vn, etc...)
		std::string command;
		file >> command;

		// Rest of part 1 here
		// We will ignore the rest of the line for comment lines
		if (command == "#")
		{
			std::getline(file, line);
		}
		// The v command defines a vertex's position
		else if (command == "v") {
			// Read in and store a position    
			file >> vecData.x >> vecData.y >> vecData.z;
			positions.push_back(vecData);
		}
		else if (command == "vt"){
			// Read in and store the texture coordinate of one vertex
			file >> uvsData.x >> uvsData.y;
			uvs.push_back(uvsData);
			
		}
		else if (command == "vn") {
			// Read in and storage normal of one vertex
			file >> normalsData.x >> normalsData.y >> normalsData.z;
			normals.push_back(normalsData);
		}
		// The f command defines a polygon in the mesh// NOTE: make sure you triangulate in blender, otherwise it will// output quads instead of triangles
		else if (command == "f") {
			// Read the rest of the line from the file 
			std::getline(file, line);
			// Trim whitespace from either end of the line
			trim(line);
			// Create a string stream so we can use streaming operators on it
			std::stringstream stream = std::stringstream(line);
			// Next slide goes here
			for (int ix = 0; ix < 3; ix++) {
				// Read in the 3 attributes (position, UV, normal)
				char separator;
				stream >> vertexIndices.x >> separator >> vertexIndices.y >> separator >> vertexIndices.z;
				// OBJ format uses 1-based indices 
				vertexIndices -= glm::ivec3(1);
				// add the vertex indices to the list
				//NOTE: This will create duplicate vertices! 
				vertices.push_back(vertexIndices);  //work on this area for objloader  
			}
		}
			
	}
		// TODO: Generate mesh from the data we loaded

		std::vector<VertexPosNormTexCol> vertexData; 
		for (int ix = 0; ix < vertices.size(); ix++) {
			glm::ivec3 attribs = vertices[ix];

			// Extract attributes from lists (except color)
			glm::vec3 position = positions[attribs.x];
			glm::vec3 normal = normals[attribs.x]; //loading normals
			glm::vec2 uv = uvs[attribs.x]; //loading UV
			glm::vec4 color = glm::vec4(1.0f);
			// from my understanding in the assignment, in this for loop, it will start from 0 so in vector 0 it be x, than 1 = y and 2 = z
			// position and normal has xyz and uv has xy
			// Add the vertex to the mesh - send new data from read file of position, normal, uv, while color stay the same.   
			vertexData.push_back(VertexPosNormTexCol(position, normal, uv, color));
		}

		// Create a vertex buffer and load all our vertex data
		VertexBuffer::Sptr vertexBuffer = VertexBuffer::Create();
		vertexBuffer->LoadData(vertexData.data(),

			vertexData.size());
		// Create the VAO, and add the Vertices
		VertexArrayObject::Sptr result = VertexArrayObject::Create();
		result->AddVertexBuffer(vertexBuffer, VertexPosNormTexCol::V_DECL);
		
		return result;
		//return VertexArrayObject::Create();
}
