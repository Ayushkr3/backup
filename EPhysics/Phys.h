#pragma once
#include "PxPhysicsAPI.h"
#include "EPhysics.h"
#include "ImGui/imgui.h"
#include "Global.h"
#include "Maths.h"
#include <memory>
#include <sstream>
extern "C" PHYSICS_API void SharedVarInit(float*&dT, bool*& inPlayMode);
//#include <DirectXMath.h>
//#include <deque>
//#include <array>
//
//class Physics_Body:public ObjectProperties {
//	std::deque < DirectX::XMFLOAT3> pendingForces;
//	float mass = 1;
//	TransformStruct*& t;
//	float*& dt;
//	float * lastdT;
//	DirectX::XMVECTOR gravityvec;
//	void ApplyDrag(std::array<float, 3>* acceleration,float velocity[3],std::array<float, 3>* last_velocity);
//public:
//	PHYSICS_API ObjectProperties* GetPropertyRef();
//	PHYSICS_API void AddForce(float x,float y,float z);
//	PHYSICS_API void inPlayMode();
//	PHYSICS_API void show();
//	PHYSICS_API Physics_Body(TransformStruct*& t, Objects*& obj,float*& dt = Globals::dT);
//	static float gravity;
//	float velocity[3] = { 0,0,0 };
//	bool isAffectedByGravity = true;
//	std::array<float, 3> acceleration = {0,0,0};
//	std::array<float, 3> last_velocity = { 0,0,0 };
//	PHYSICS_API void Update();
//	PHYSICS_API void ResetVelocity();
//	//TEST CODES GOES HERE
//	PHYSICS_API static void ResetGravity();
//};
//extern "C" PHYSICS_API Physics_Body* CreatePhysicsBody(TransformStruct* t,Objects*& obj);
//
//
class NVPhysx{
private:
	static physx::PxDefaultAllocator allocator;
	static physx::PxDefaultErrorCallback errorCallback;
	static physx::PxPvdTransport* transport;
	static physx::PxPvd* pvd;
public:
	PHYSICS_API static ImGuiContext* privateCtx;
	PHYSICS_API static void CreateNewScene(physx::PxScene*& Scene);
	PHYSICS_API static physx::PxFoundation* foundation;
	PHYSICS_API static physx::PxPhysics* physicsObj;
	PHYSICS_API static void Init();
	PHYSICS_API static void Destroy();
	class RigidBody:public ObjectProperties {
	private:
		bool isInitalized = false ;
		TransformStruct* nulltrans;
		Euler updatedAngle;
	private:
		bool isAffectedbyGravity = true;
		bool isStaticObject = false;
		physx::PxMaterial* objectMaterial;
	public:
		TransformStruct* trans;
		PHYSICS_API void InitPlayMode();
		PHYSICS_API void DeInitPlayMode();
		PHYSICS_API static void RegisterFactory(std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>& GlobalPropertiesPoolL = ObjectProperties::GlobalPropertiesPool);
		PHYSICS_API void UpdateDependency(const void* ptr);
		PHYSICS_API void inPlayMode();
		physx::PxRigidDynamic* DynamicActor;
		physx::PxRigidStatic* StaticActor;
		PHYSICS_API RigidBody(Objects* obj);
		void UpdatePhysics();
		PHYSICS_API void show();
		PHYSICS_API ObjectProperties* GetPropertyRef();
		PHYSICS_API physx::PxActor* GetCurrentActor();
		PHYSICS_API const std::type_info& GetPropertyType();
		PHYSICS_API std::string GetPropertyClassName() { return "RigidBody"; }
	};
	class Collider:public ObjectProperties{
		RigidBody* rb;
		physx::PxMaterial* Material;
		physx::PxShape* shape;
		bool isInitalized = false;
	public:
		PHYSICS_API const std::type_info& GetPropertyType();
		PHYSICS_API ObjectProperties* GetPropertyRef();
		PHYSICS_API static void RegisterFactory(std::multimap<std::string, std::function<ObjectProperties*(Objects*)>>& GlobalPropertiesPoolL = ObjectProperties::GlobalPropertiesPool);
		PHYSICS_API void InitPlayMode();
		PHYSICS_API void DeInitPlayMode();
		PHYSICS_API void show();
		PHYSICS_API void UpdateDependency(const void* ptr);
		PHYSICS_API Collider(Objects* obj);
		PHYSICS_API std::string GetPropertyClassName() { return "Collider"; }
	};
};
extern "C" PHYSICS_API ObjectProperties* CreateRigidBody(Objects* obj);
PHYSICS_API ObjectProperties* CreateCollider(Objects* obj);