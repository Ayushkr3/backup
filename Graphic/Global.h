#pragma once
struct TransformStruct {
	float rotation[3] = { 0,0,0 };
	float position[3] = { 0,0,0 };
	float Scale[3] = { 1,1,1 };
};
class Globals {
public:
	static float* dT;
};
struct Vertex {
	struct {
		float x;
		float y;
		float z;
	}position;
	struct {
		float u;
		float v;
	}texcoord;
	struct {
		float x;
		float y;
		float z;
	}Normal;
};
