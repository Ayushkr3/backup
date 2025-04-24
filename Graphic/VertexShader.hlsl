struct VSOut
{
	//Here fucking order matters do not make random variable or it will fuck you
	///Order should be similiar to Mains input and order of SV semantic can be ignored
	//float3 n_vector : Normal;
	//float4 positionForCalculation :Position;
	float3 pos : Position1;
	float2 uv : Texcoord;
};


VSOut main(float3 pos : Position,float2 tex: Texcoord,float3 norm:Normal)
{
	VSOut vso;
	//float4x4 transformationMatrix = mul(Scale, mul(Rotation, Translation));
	//float4 TransformedMat = mul(float4(pos, 1.0f), transformationMatrix);
	//vso.pos = mul(TransformedMat, viewXprojection).xyz;
	vso.pos = pos;
	vso.uv = tex;
	//vso.n_vector = (mul(float4(norm, 1.0f),mul(Rotation, Translation))).xyz;
	//vso.positionForCalculation = float4(pos, 1.0f);
	return vso;
}