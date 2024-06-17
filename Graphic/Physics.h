#include <d3d11.h>
#include "ConstantBuff.h"
#include "Global.h"
#include <array>
class Triangle;

class Physics_Body {
	float* dt;
	float objTrans[3] = { 0,0,0 };
	float lastPos[3] = {0,0,0};
	float g_velocity = 0;
	float mass = 1;
public:
	Physics_Body();
	static float gravity;
	float velocity[3] = { 0,0,0};
	bool isMoving();
	bool isAffectedByGravity = true;
	void Update(float* positionX, float* positionY, float* positionZ);
};