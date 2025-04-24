#pragma once
#include "EngInc/NVPhysx/PxPhysicsAPI.h"
#include <iostream>
#include <cmath>
struct Euler {
	float x;
	float y;
	float z;
};
struct Quaternion {
	float x;
	float y;
	float z;
	float w;
	Quaternion(const physx::PxQuat vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
	}
	Quaternion(float x,float y,float z,float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	Quaternion& operator=(const physx::PxQuat vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;
		return *this;
	}
};
Euler FromQuaternionToEuler(Quaternion q);
physx::PxQuat EulerToQuaternion(float yaw, float pitch, float roll);