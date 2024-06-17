struct VSOut
{
	float3 color : Color;
	float4 pos : SV_Position;
};
cbuffer Test: register (b0){
	 row_major matrix Rotation;
	 row_major matrix Translation;
	 row_major matrix Scale;
}
cbuffer PerFrameBuffer : register (b1) {
	  matrix viewXprojection;
}

VSOut main(float3 pos : Position, float3 color : Color)
{
	VSOut vso;
	float4x4 transformationMatrix = mul(Scale,mul(Rotation,Translation));
	float4 TransformedMat = mul(float4(pos, 1.0f), transformationMatrix);
	vso.pos = mul(TransformedMat, viewXprojection);
	vso.color = color;
	return vso;
}