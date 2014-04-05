// ModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

typedef struct
{
	float x, y, z;
}VertexType;

typedef struct
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
}FaceType; //?

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
void GetModelFilename(char*);
bool ReadFileCounts(char*, int&, int&, int&, int&);
bool LoadDataStructures(char*, int, int, int, int);


int _tmain(int argc, _TCHAR* argv[])
{
	char filename[256] = "C:\\08961-RealTime\\dx11tut03\\Engine\\Engine\\data\\uvtest.obj";
	char test[256];

	bool result;
	int vertexCount, textureCount, normalCount, faceCount;
	char garbage;

	GetModelFilename(test);

	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
	{
		return -1;
	}
	cout << " vertex count!  " << vertexCount;
	cout << " normal count! " << normalCount;
	cout << " UV count! " << textureCount;

	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if(!result)
	{
		return -1;
	}

	cin >> test;
	return 0;
}

void GetModelFilename(char* filename)
{
	bool done;
	ifstream fin;

	// Loop until we have a file name.
	done = false;
	while(!done)
	{

		TCHAR buffer[260];
		GetModuleFileName( NULL, buffer, 260 );
		// Ask the user for the filename.
		cout << "Enter model filename: ";

		// Read in the filename.
		cin >> filename;
		
		// Attempt to open the file.
		fin.open("C:\\08961-RealTime\\dx11tut03\\Engine\\Engine\\data\\untitled.obj");

		if(fin.good())
		{
			// If the file exists and there are no problems then exit since we have the file name.
			done = true;
			cout << "file opened";
			filename = "C:\\08961-RealTime\\dx11tut03\\Engine\\Engine\\data\\cube.txt";
		}
		else
		{
			// If the file does not exist or there was an issue opening it then notify the user and repeat the process.
			fin.clear();
			cout << endl;
			cout << "File " << filename << " could not be opened." << endl << endl;
		}
	}

	return;
}

bool ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	//clear and initilize
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	fin.get(input);
	while(!fin.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if(input == 'v')
		{
			fin.get(input);
			if(input == ' ') { vertexCount++; }
			if(input == 't') { textureCount++; }
			if(input == 'n') { normalCount++; }
		}

		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			fin.get(input);
			if(input == ' ') { faceCount++; }
		}
		
		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);

	}
	return true;
}

bool LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType *vertices, *texcoords, *normals;
	FaceType *faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;

	// Initialize the four data structures.
	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	texcoords = new VertexType[textureCount];
	if(!texcoords)
	{
		return false;
	}

	normals = new VertexType[normalCount];
	if(!normals)
	{
		return false;
	}

	faces = new FaceType[faceCount];
	if(!faces)
	{
		return false;
	}

	// Initialize the indexes.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// Open the file.
	fin.open(filename);

	// Check if it was successful in opening the file.
	if(fin.fail() == true)
	{
		return false;
	}

	// Read in the vertices, texture coordinates, and normals into the data structures.
	// Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
	fin.get(input);
	while(!fin.eof())
	{
		if(input == 'v')
		{
			fin.get(input);

			// Read in the vertices.
			if(input == ' ') 
			{ 
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++; 
			}

			// Read in the texture uv coordinates.
			if(input == 't') 
			{ 
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++; 
			}

			// Read in the normals.
			if(input == 'n') 
			{ 
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Invert the Z normal to change to left hand system.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++; 
			}
		}

		//// Read in the faces.
		//if(input == 'f') 
		//{
		//	fin.get(input);
		//	if(input == ' ')
		//	{
		//		// Read the face data in backwards to convert it to a left hand system from right hand system.
		//		fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
		//		    >> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
		//		    >> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
		//		faceIndex++;
		//	}
		//}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			fin.get(input);
		}

		// Start reading the beginning of the next line.
		fin.get(input);
	}

	return true;
}