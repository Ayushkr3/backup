#pragma once
#include "EPhysics.h"
#include "Global.h"
#include <DirectXMath.h>
#include <deque>
class Physics_Body:public ObjectProperties {
	std::deque < DirectX::XMFLOAT3> pendingForces;
	float objTrans[3] = { 0,0,0 };
	float lastPos[3] = { 0,0,0 };
	float g_velocity = 0;
	float mass = 1;
	TransformStruct*& t;
	float*& dt;
	float * lastdT;
	DirectX::XMVECTOR gravityvec;
public:
	PHYSICS_API ObjectProperties* GetPropertyRef();
	PHYSICS_API void AddForce(float x,float y,float z);
	PHYSICS_API void inPlayMode();
	PHYSICS_API void show();
	PHYSICS_API Physics_Body(TransformStruct*& t,float*& dt = Globals::dT);
	static float gravity;
	float velocity[3] = { 0,0,0 };
	bool isAffectedByGravity = true;
	PHYSICS_API void Update();
	PHYSICS_API void ResetVelocity();
	//TEST CODES GOES HERE
	PHYSICS_API static void ResetGravity();
};
extern "C" PHYSICS_API Physics_Body* CreatePhysicsBody(TransformStruct* t);