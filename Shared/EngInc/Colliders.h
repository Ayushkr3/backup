//#pragma once
//#ifndef COLLIDERS_H
//#define COLLIDERS_H
//#include "EPhysics.h"
//#include <d3d11.h>
//#include <vector>
//#include <DirectXMath.h>
//#include <algorithm>
//#include "ImGui/imgui.h"
//#include "Phys.h"
//#include "Global.h"
//namespace Collision {
//	struct Projection {
//		float maxi = -FLT_MAX;
//		float mini = FLT_MAX;
//	};
//	struct AABBProjection {
//		float maxI = -FLT_MAX;
//		float minI = FLT_MAX;
//		float maxJ = -FLT_MAX;
//		float minJ = FLT_MAX;
//		float maxK = -FLT_MAX;
//		float minK = FLT_MAX;
//		void operator*(int num){
//			 maxI *=num;
//			 minI *=num;
//			 maxJ *=num;
//			 minJ *=num;
//			 maxK *=num;
//			 minK *=num;
//		}
//	};
//}
////cant export whole class with STL being in it as cited
////https://learn.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4251?view=msvc-170
//class BoxCollider:public ObjectProperties {
//private:
//	Physics_Body* phys;
//	TransformStruct*& objTransform;
//	ImGuiContext* cxt;
//	std::vector<NormalPerObject> ObjectsNormals;
//	bool Comp(DirectX::XMFLOAT3& v1, DirectX::XMFLOAT3& v2);
//	std::vector<Vertex> vert;
//	DirectX::XMFLOAT3 lastCollaxis;
//	struct axis {
//		std::vector<DirectX::XMFLOAT3> normals;
//	};
//	Collision::Projection CalcProjection(DirectX::XMFLOAT3 normals);
//public:
//	bool isStaticObject = false;
//	PHYSICS_API void ResolveCollision();
//	PHYSICS_API ObjectProperties* GetPropertyRef();
//	PHYSICS_API void inPlayMode() {};
//	PHYSICS_API void show();
//	PHYSICS_API BoxCollider(TransformStruct*& trans, std::vector<Vertex>& vertices, std::vector<NormalPerObject> Normals, Objects*& obj, ImGuiContext* ctx = ImGui::GetCurrentContext());
//	//std::vector<Collision::Projection> p1;
//	//std::vector<Collision::Projection> p2;
//	axis axis1;
//	PHYSICS_API void CalcAxis(std::vector<NormalPerObject>&Nor);
//	PHYSICS_API bool CheckCollision(BoxCollider secondObj);
//	PHYSICS_API void UpdateBuffer(std::vector<Vertex>& vertices, std::vector<NormalPerObject>& n);
//private:
//	bool CheckCollisionSAT(const std::vector<Collision::Projection>& cubeProjections, const std::vector<Collision::Projection>& planeProjections);
//	//bool isColliding = true;
//	bool IsOverlap(float minA, float maxA, float minB, float maxB);
//public:
//	//float* test;
//};
//extern "C" PHYSICS_API BoxCollider *CreateBoxCollider(Objects*& obj,TransformStruct*& trans,std::vector<Vertex>& vertices, std::vector<NormalPerObject> Normals);
//#endif