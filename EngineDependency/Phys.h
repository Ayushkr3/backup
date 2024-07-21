#include "EPhysics.h"
#include "Global.h"

class Physics_Body:public ObjectProperties {
	float objTrans[3] = { 0,0,0 };
	float lastPos[3] = { 0,0,0 };
	float g_velocity = 0;
	float mass = 1;
	TransformStruct*& t;
	float*& dt;
public:
	PHYSICS_API void inPlayMode();
	PHYSICS_API void show();
	PHYSICS_API Physics_Body(TransformStruct*& t,float*& dt = Globals::dT);
	static float gravity;
	float velocity[3] = { 0,0,0 };
	bool isAffectedByGravity = true;
	PHYSICS_API void Update();
	//TEST CODES GOES HERE
	PHYSICS_API static void ResetGravity();
};
extern "C" PHYSICS_API Physics_Body* CreatePhysicsBody(TransformStruct* t);