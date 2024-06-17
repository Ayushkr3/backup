#pragma once
#include <d3d11.h>
#include <vector>
#include "Global.h"
#include <DirectXMath.h>
#include <algorithm>
namespace Collision {
	struct Projection {
		float maxi = -FLT_MAX;
		float mini = FLT_MAX;
	};
	struct AABBProjection {
		float maxI = -FLT_MAX;
		float minI = FLT_MAX;
		float maxJ = -FLT_MAX;
		float minJ = FLT_MAX;
		float maxK = -FLT_MAX;
		float minK = FLT_MAX;
		void operator*(int num){
			 maxI *=num;
			 minI *=num;
			 maxJ *=num;
			 minJ *=num;
			 maxK *=num;
			 minK *=num;
		}
	};
}
class BoxCollider {
private:

	struct axis {
		std::vector<DirectX::XMFLOAT3> normals;
	};
	std::vector<Collision::Projection> CalcProjection(std::vector<DirectX::XMFLOAT3> normals);
public:
	BoxCollider(std::vector<Vertex> vertices);
	std::vector<Collision::Projection> p1;
	std::vector<Collision::Projection> p2;
	axis axis1;
	std::vector <std::vector<Vertex>> faces;
	void CalcAxis(std::vector<Vertex>& vertices);
	bool CheckCollision(BoxCollider secondObj);
	void UpdateBuffer(std::vector<Vertex>& vertices);
private:
	bool CheckCollisionSAT(const std::vector<Collision::Projection>& cubeProjections, const std::vector<Collision::Projection>& planeProjections);
	bool isColliding ;
	bool isMoving;
	bool IsOverlap(float minA, float maxA, float minB, float maxB);
public:
	//float* test;
};
