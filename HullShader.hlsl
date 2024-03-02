
struct HS_CONTROL_POINT_INPUT {
	float3 pos : Position1;
};
struct HS_CONTROL_POINT_OUTPUT {
	float3 pos : Position1;
};
struct HS_CONSTANT_DATA {
	float EdgeTessFactor[3] : SV_TessFactor;
	float InsideTessFactor : SV_InsideTessFactor;
};

HS_CONSTANT_DATA HullConstantDataOutput(InputPatch<HS_CONTROL_POINT_INPUT, 3> Ip,uint PatchID : SV_PrimitiveID) {
	HS_CONSTANT_DATA Cons;
	Cons.EdgeTessFactor[0] = 2;
	Cons.EdgeTessFactor[1] = 2;
	Cons.EdgeTessFactor[2] = 2;
	Cons.InsideTessFactor = 1;
	return Cons;
}
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HullConstantDataOutput")]


HS_CONTROL_POINT_INPUT main(InputPatch<HS_CONTROL_POINT_INPUT,3> Ip, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID) {
	HS_CONTROL_POINT_OUTPUT ho;
	ho.pos = (Ip[i].pos);
	return ho;
}