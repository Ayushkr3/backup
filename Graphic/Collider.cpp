#include "Collider.h"
//SAT algorithm
//TODO: Consider Tessllated normals (Barrycentric normals)
using namespace DirectX;
using namespace Collision;
BoxCollider::BoxCollider(std::vector<Vertex> vertices) {
	CalcAxis(vertices);
}
std::vector<Projection> BoxCollider::CalcProjection(std::vector<DirectX::XMFLOAT3> normals) {
	std::vector<Projection> allProj;
	Projection pro;
	for (unsigned int i = 0; i < normals.size(); i++) {
		pro.mini = FLT_MAX;
		pro.maxi = -FLT_MAX;
		auto vec = DirectX::XMLoadFloat3(&(normals)[i]);
		for (unsigned int k = 0; k < faces.size(); k++) {
			float maxi = -FLT_MAX;
			float mini = FLT_MAX;
			for (unsigned int j = 0; j < faces[k].size(); j++) {
				auto pos = XMFLOAT3(faces[k][j].position.x, faces[k][j].position.y, faces[k][j].position.z);
				float proj = XMVectorGetX(XMVector3Dot(vec, XMLoadFloat3(&pos)));
				 maxi = max(proj,maxi);
				 mini = min(proj,mini);
			}
			pro.maxi = max(maxi, pro.maxi);
			pro.mini = min(mini, pro.mini);
		}
		allProj.push_back(pro);
	}
	return allProj;
}
void BoxCollider::CalcAxis(std::vector<Vertex>& vertices) {
	std::vector<Vertex> face;
		for (unsigned int i = 0; i < (vertices).size(); i = i + 4) {
			face.push_back((vertices)[i]);
			face.push_back((vertices)[i + 1]);
			face.push_back((vertices)[i + 2]);
			face.push_back((vertices)[i + 3]);
			faces.push_back(face);
			auto nor = DirectX::XMFLOAT3((vertices)[i].Normal.x, vertices[i].Normal.y, vertices[i].Normal.z);
			//auto vec = DirectX::XMLoadFloat3(&nor);
			bool present = false;
			axis1.normals.push_back(nor);
			face.clear();
		}
		//*test = faces[0][0].position.x;
}
void BoxCollider::UpdateBuffer(std::vector<Vertex>& vertices) {
	int k = 0;
	for (unsigned int i = 0; i < faces.size(); i++) {
		for (unsigned int j = 0; j < 3; j=j+4) {
			faces[i][j] = vertices[k];
			faces[i][j+1] = vertices[k+1];
			faces[i][j+2] = vertices[k+2];
			faces[i][j+3] = vertices[k+3];
			auto nor = DirectX::XMFLOAT3((vertices)[k].Normal.x, vertices[k].Normal.y, vertices[k].Normal.z);
			axis1.normals[i] = nor;
			k = k + 4;
		}
		//auto vec = DirectX::XMLoadFloat3(&nor);
	}
	k = 0;
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
	bool test = (minA <= maxB && maxA >= minB);
	return test;
}

////////////////////--------------------//////////////////////