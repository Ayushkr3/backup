struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	// TODO: change/add other stuff
};
cbuffer Test: register (b0) {
	row_major matrix Rotation;
	row_major matrix Translation;
	row_major matrix Scale;
}
cbuffer PerFrameBuffer : register (b1) {
	matrix viewXprojection;
}
 //Output control point
struct HS_CONTROL_POINT_OUTPUT
{
	float3 pos : Position1;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor; // e.g. would be [4] for a quad domain
	float InsideTessFactor			: SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
	// TODO: change/add other stuff
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;
	float4 vPosition = float4(
		patch[0].pos*domain.x + patch[1].pos*domain.y + patch[2].pos*domain.z, 1);
	float4x4 transformationMatrix = mul(Scale, mul(Rotation, Translation));
	float4 TransformedMat = mul(vPosition, transformationMatrix);
	Output.vPosition = mul(TransformedMat, viewXprojection);

	return Output;
}
