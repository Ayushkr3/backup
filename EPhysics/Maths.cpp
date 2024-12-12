#include "pch.h"
#include "Maths.h"
#define _USE_MATH_DEFINES
Euler FromQuaternionToEuler(Quaternion q) {
	Euler angle;
	float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	angle.x = std::atan2(sinr_cosp, cosr_cosp);

	float sinp = 2.0f * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		angle.y = std::copysign(physx::PxPi / 2, sinp); // use 90 degrees if out of range
	else
		angle.y = std::asin(sinp);

	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angle.z = std::atan2(siny_cosp, cosy_cosp);

	return angle;
}
physx::PxQuat EulerToQuaternion(float yaw, float pitch, float roll) {
	float cy = std::cos(yaw * 0.5f);
	float sy = std::sin(yaw * 0.5f);
	float cp = std::cos(pitch * 0.5f);
	float sp = std::sin(pitch * 0.5f);
	float cr = std::cos(roll * 0.5f);
	float sr = std::sin(roll * 0.5f);
	return physx::PxQuat(
		sr * cp * cy - cr * sp * sy, // x
		cr * sp * cy + sr * cp * sy, // y
		cr * cp * sy - sr * sp * cy, // z
		cr * cp * cy + sr * sp * sy  // w
	);
}