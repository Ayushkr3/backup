Texture2D obj_tex : register(t1);
SamplerState obj_Sampler : register(s1);

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : Texcoord0;
	float3 vertices:Position1;
};
 
float4 main(PSInput PI) : SV_TARGET
{
	float4 color = obj_tex.Sample(obj_Sampler, PI.uv);
	return color;
}