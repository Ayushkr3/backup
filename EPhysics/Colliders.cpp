#include "pch.h" //Should be included at top :'( wasted 5 hours
#include "Colliders.h"
#include "EPhysics.h"
#include "ImGui/imgui.h"
//SAT algorithm
//TODO: Fix Normal update
using namespace DirectX;
using namespace Collision;
BoxCollider::BoxCollider(TransformStruct*& trans,std::vector<Vertex>& vertices, std::vector<NormalPerObject> Normals, ImGuiContext * ctx) :vert(vertices), ObjectsNormals{Normals},cxt(ctx), objTransform(trans) {
	CalcAxis(Normals);
	ImGui::SetCurrentContext(ctx);
}
Collision::Projection BoxCollider::CalcProjection(DirectX::XMFLOAT3 normals) {
	//std::vector<Projection> allProj;
	Projection pro;
		pro.mini = FLT_MAX;
		pro.maxi = -FLT_MAX;
		auto vec = DirectX::XMLoadFloat3(&(normals));
		for (unsigned int k = 0; k < vert.size(); k++) {
			float maxi = -FLT_MAX;
			float mini = FLT_MAX;
			auto pos = XMFLOAT3(vert[k].position.x, vert[k].position.y, vert[k].position.z);
			float proj = XMVectorGetX(XMVector3Dot(vec, XMLoadFloat3(&pos)));
			maxi = max(proj, maxi);
			mini = min(proj, mini);
			pro.maxi = max(maxi, pro.maxi);
			pro.mini = min(mini, pro.mini);
		}
	return pro;
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
	bool isColliding = true;
	std::vector<Collision::Projection> allproj;
	DirectX::XMFLOAT3 collaxis;
	float minover = std::numeric_limits<float>::infinity();
	for (unsigned int i = 0; i < axis1.normals.size();i++) {
		Projection p1;
		Projection p2;
		p1 = CalcProjection(axis1.normals[i]);
		p2 = secondObj.CalcProjection(axis1.normals[i]);
		if (IsOverlap(p1.mini, p1.maxi, p2.mini, p2.maxi)) {
			
			float overlap = min(p1.maxi, p2.maxi) - max(p1.mini, p2.mini);
			isColliding = isColliding && true;
			if (minover > overlap) {
				collaxis = axis1.normals[i];
				minover = overlap;
			}
		}
		else {
			isColliding = isColliding && false;
		}
	}
	if (isColliding && !isStaticObject) {
		objTransform->position[0] = objTransform->position[0] + (collaxis.x*minover);
		objTransform->position[1] = objTransform->position[1] + (collaxis.y*minover);
		objTransform->position[2] = objTransform->position[2] + (collaxis.z*minover);

	}
	 //p1 = CalcProjection(axis1.normals);
	 //p2 = secondObj.CalcProjection(axis1.normals); // potential to remove self projection
	// isColliding = CheckCollisionSAT(p1, p2);
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
void BoxCollider::show() {
	if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_CollapsingHeader)) {
		ImGui::Checkbox("Static Object", &isStaticObject);
	}
}
////////////////////--------------------//////////////////////

BoxCollider* CreateBoxCollider(TransformStruct*& trans, std::vector<Vertex>& vertices, std::vector<NormalPerObject> Normals)
{
	return new BoxCollider(trans,vertices,Normals);
}
