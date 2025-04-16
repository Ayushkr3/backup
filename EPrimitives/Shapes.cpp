#include "pch.h"
#include "Shapes.h"
using namespace Primitives;
using namespace DirectX;
void BasicShapes::Circle::GetCircleofLongNLat(short Lat, short Long, std::vector<Vertex>& vertices, std::vector<unsigned int>& index) {
	auto NumSphereVertices = ((Lat - 2) * Long) + 2;
	auto NumSphereFaces = ((Lat - 3)*(Long) * 2) + (Long * 2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;
	vertices.resize(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 1.0f;

	for (DWORD i = 0; i < Lat - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (Lat - 1));
		auto Rotationx = XMMatrixRotationX(spherePitch);
		for (DWORD j = 0; j < Long; ++j)
		{
			sphereYaw = j * (6.28 / (Long));
			auto Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
			currVertPos = XMVector3Normalize(currVertPos);
			vertices[i*Long + j + 1].position.x = XMVectorGetX(currVertPos);
			vertices[i*Long + j + 1].position.y = XMVectorGetY(currVertPos);
			vertices[i*Long + j + 1].position.z = XMVectorGetZ(currVertPos);
		}
	}
	vertices[NumSphereVertices - 1].position.x = 0.0f;
	vertices[NumSphereVertices - 1].position.y = 0.0f;
	vertices[NumSphereVertices - 1].position.z = -1.0f;
	index.resize(NumSphereFaces * 3);

	int k = 0;
	for (DWORD l = 0; l < Long - 1; ++l)
	{
		index[k] = 0;
		index[k + 1] = l + 1;
		index[k + 2] = l + 2;
		k += 3;
	}

	index[k] = 0;
	index[k + 1] = Long;
	index[k + 2] = 1;
	k += 3;

	for (DWORD i = 0; i < Lat - 3; ++i)
	{
		for (DWORD j = 0; j < Long - 1; ++j)
		{
			index[k] = i * Long + j + 1;
			index[k + 1] = i * Long + j + 2;
			index[k + 2] = (i + 1)*Long + j + 1;

			index[k + 3] = (i + 1)*Long + j + 1;
			index[k + 4] = i * Long + j + 2;
			index[k + 5] = (i + 1)*Long + j + 2;

			k += 6; // next quad
		}

		index[k] = (i*Long) + Long;
		index[k + 1] = (i*Long) + 1;
		index[k + 2] = ((i + 1)*Long) + Long;

		index[k + 3] = ((i + 1)*Long) + Long;
		index[k + 4] = (i*Long) + 1;
		index[k + 5] = ((i + 1)*Long) + 1;

		k += 6;
	}

	for (DWORD l = 0; l < Long - 1; ++l)
	{
		index[k] = NumSphereVertices - 1;
		index[k + 1] = (NumSphereVertices - 1) - (l + 1);
		index[k + 2] = (NumSphereVertices - 1) - (l + 2);
		k += 3;
	}

	index[k] = NumSphereVertices - 1;
	index[k + 1] = (NumSphereVertices - 1) - Long;
	index[k + 2] = NumSphereVertices - 2;

}
void BasicShapes::Cube::GetBox(std::vector<Vertex>& vertices, std::vector<unsigned int>& index) {
	std::vector<Vertex> vertice = { {-1.0f, -1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f, -1.0f, 0.0f, 0.0f}, // Top-left

	// Back face
	{-1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-left

	// Left face
	{-1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-left
	{-1.0f, -1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{-1.0f,  1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-left

	// Right face
	{ 1.0f, -1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-right
	{ 1.0f,  1.0f, -1.0f, 0.0f, 0.0f}, // Top-left

	// Top face
	{-1.0f,  1.0f, -1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f,  1.0f, -1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f,  1.0f,  1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f,  1.0f,  1.0f, 0.0f, 0.0f}, // Top-left

	// Bottom face
	{-1.0f, -1.0f,  1.0f, 0.0f, 1.0f}, // Bottom-left
	{ 1.0f, -1.0f,  1.0f, 1.0f, 1.0f}, // Bottom-right
	{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f}, // Top-right
	{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f} };
	std::vector<unsigned int> indices = { 0, 2, 1,  0, 3, 2,
		// Back face
		4, 5, 6,  4, 6, 7,
		// Left face
		8, 10, 9, 8, 11, 10,
		// Right face
		12, 14, 13, 12, 15, 14,
		// Top face
		16, 18, 17, 16, 19, 18,
		// Bottom face
		20, 22, 21, 20, 23, 22 };
	vertices = vertice;
	index = indices;
}

void Primitives::BasicShapes::Plane::GetPlane(std::vector<Vertex>& vertices, std::vector<unsigned int>& index)
{
	std::vector<Vertex> vertice = { {1.0f,0.0f,1.0f,1.0f,0.0f},
	{-1.0f,0.0f,-1.0f,0.0f,1.0f },
	{-1.0f,0.0f,1.0f,0.0f,0.0f },
	{1.0f,0.0f,1.0f,1.0f,0.0f },
	{1.f,0.f,-1.f,1.f,1.f},
	{-1.0f,0.0f,-1.0f,0.0f,1.0f } };
	std::vector<unsigned int> indi = { 0,1,2,3,4,5 };
	vertices = vertice;
	index = indi;
}
