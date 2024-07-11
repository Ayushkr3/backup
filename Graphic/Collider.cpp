#include "Collider.h"
//SAT algorithm
//TODO: Fix Normal update
using namespace DirectX;
using namespace Collision;
BoxCollider::BoxCollider(std::vector<Vertex>& vertices,std::vector<NormalPerObject> Normals) :vert(vertices), ObjectsNormals{Normals} {
	CalcAxis(Normals);
}
std::vector<Projection> BoxCollider::CalcProjection(std::vector<DirectX::XMFLOAT3> normals) {
	std::vector<Projection> allProj;
	Projection pro;
	for (unsigned int i = 0; i < normals.size(); i++) {
		pro.mini = FLT_MAX;
		pro.maxi = -FLT_MAX;
		auto vec = DirectX::XMLoadFloat3(&(normals)[i]);
		for (unsigned int k = 0; k < vert.size(); k++) {
			float maxi = -FLT_MAX;
			float mini = FLT_MAX;
			auto pos = XMFLOAT3(vert[k].position.x, vert[k].position.y, vert[k].position.z);
			float proj = XMVectorGetX(XMVector3Dot(vec, XMLoadFloat3(&pos)));
			maxi = max(proj,maxi);
			mini = min(proj,mini);
			pro.maxi = max(maxi, pro.maxi);
			pro.mini = min(mini, pro.mini);
		}
		allProj.push_back(pro);
	}
	return allProj;
}
bool BoxCollider::Comp(DirectX::XMFLOAT3& v1, DirectX::XMFLOAT3& v2) {
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
		return true;
	return false;
}
void BoxCollider::CalcAxis(std::vector<NormalPerObject>& Nor) {
	if (Nor.empty()) {
		return;
	}
	for (unsigned int i = 0; i < (Nor).size(); i++) {
		auto nor = DirectX::XMFLOAT3(Nor[i].x, Nor[i].y, Nor[i].z);
		axis1.normals.push_back(nor);
	}
}
void BoxCollider::UpdateBuffer(std::vector<Vertex>& vertices,std::vector<NormalPerObject>& n) {
	vert = vertices;
	for (unsigned int i = 0; i < n.size(); i++) {
			auto nor = DirectX::XMFLOAT3((n)[i].x, n[i].y, n[i].z);
			axis1.normals[i] = nor;
		//auto vec = DirectX::XMLoadFloat3(&nor);
	}
	//axis1.normals = std::vector<XMFLOAT3>(std::set<XMFLOAT3>(temp.begin(),temp.end()).begin(),std::set<XMFLOAT3>(temp.begin(), temp.end()).end());
}
////////////////////----Narrow Phase-----/////////////////////////////
bool BoxCollider::CheckCollision(BoxCollider secondObj) {
	 p1 = CalcProjection(secondObj.axis1.normals);
	 p2 = secondObj.CalcProjection(secondObj.axis1.normals);
	 isColliding = CheckCollisionSAT(p1, p2);

	 //get the projection of 1 object on 2 object on axis --- Done p1 p2
	 //p1 the projection of itself on its normal
	 //compare p2 with p1 of the other object 
	 return isColliding;
}
bool BoxCollider::CheckCollisionSAT(const std::vector<Projection>& cubeProjections, const std::vector<Projection>& planeProjections)
{
	for (unsigned int i = 0; i < cubeProjections.size();i++) {
			if (!IsOverlap(cubeProjections[i].mini, cubeProjections[i].maxi, planeProjections[i].mini, planeProjections[i].maxi)) {
				return false;
			}
	}
	return true;
}

bool  BoxCollider::IsOverlap(float minA, float maxA, float minB, float maxB)
{
	return (minA <= maxB && maxA >= minB);
}

////////////////////--------------------//////////////////////