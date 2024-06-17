Texture2D obj_tex : register(t0);
SamplerState obj_Sampler : register(s0);

cbuffer lightData : register(b2) {
	float4 position;
}

float4 main(/*float2 tex : Texcoord, float3 norm : Normal, float4 positionForCalculation : Position*/) :SV_TARGET
{
//float cosineAngle = max(dot(normalize(float4(norm,1.0f)),normalize(position - positionForCalculation)),0.0f);
//float4 texColor = obj_tex.Sample(obj_Sampler, tex);
//float4 ambient = (0.3*texColor);
//float4 finalcolor = ambient+(texColor * cosineAngle);
float4 test = float4(1, 1, 1, 1);
return test;
}