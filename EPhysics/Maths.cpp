#include "pch.h"
#include "Maths.h"
#include <cmath>
#define _USE_MATH_DEFINES
Euler FromQuaternionToEuler(Quaternion q) {
	Euler angle;
	float magnitude = std::sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
	q.w /= magnitude;
	q.x /= magnitude;
	q.y /= magnitude;
	q.z /= magnitude;
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	angle.x = std::atan2(sinr_cosp, cosr_cosp);
	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		angle.y = std::copysign(physx::PxPi / 2, sinp); // use 90 degrees if out of range
	else
		angle.y = std::asin(sinp);
	/*float sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	float cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angle.y = 2 * std::atan2(sinp, cosp) - physx::PxPi / 2;*/


	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angle.z = std::atan2(siny_cosp, cosy_cosp);
	angle.x = angle.x*(180.0 / physx::PxPi);
	angle.y = angle.y*(180.0 / physx::PxPi);
	angle.z = angle.z*(180.0 / physx::PxPi);
	return angle;
}
physx::PxQuat EulerToQuaternion(float yaw, float pitch, float roll) {
	yaw = yaw * physx::PxPi / 180.0f;
	pitch = pitch * physx::PxPi / 180.0f;
	roll = roll * physx::PxPi / 180.0f;
	float cy = std::cos(yaw * 0.5f);
	float sy = std::sin(yaw * 0.5f);
	float cp = std::cos(pitch * 0.5f);
	float sp = std::sin(pitch * 0.5f);
	float cr = std::cos(roll * 0.5f);
	float sr = std::sin(roll * 0.5f);
	//return physx::PxQuat(
	//	sr * cp * cy - cr * sp * sy, // x
	//	cr * sp * cy + sr * cp * sy, // y
	//	cr * cp * sy - sr * sp * cy, // z
	//	cr * cp * cy + sr * sp * sy  // w
	//);
	float x = sr * cp * cy - cr * sp * sy;
	float y = cr * sp * cy + sr * cp * sy;
	float z = cr * cp * sy - sr * sp * cy;
	float w = cr * cp * cy + sr * sp * sy;
	physx::PxQuat q = physx::PxQuat(x,y,z,w);
	q.normalize();
	return q;
}