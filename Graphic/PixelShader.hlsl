Texture2D obj_tex : register(t0);
SamplerState obj_Sampler : register(s0);

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv : Texcoord0;
	float3 vertices:Position1;
};
cbuffer lightData : register(b2) {
	float4 position;
}
cbuffer col : register(b3) {
	float3 abc;
}
float4 main(PSInput PI/*, float3 norm : Normal, float4 positionForCalculation : Position*/) :SV_TARGET
{
//float cosineAngle = max(dot(normalize(float4(norm,1.0f)),normalize(position - positionForCalculation)),0.0f);
//float4 texColor = obj_tex.Sample(obj_Sampler, PI.uv);
//float4 ambient = (0.3*texColor);
//float4 finalcolor = ambient+(texColor * cosineAngle);
float4 test = float4(abc, 1);
return test;
}