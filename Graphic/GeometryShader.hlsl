struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 uv: Texcoord0;
	float3 vertices:Position1;
};
cbuffer PerFrameBuffer : register (b1) {
	matrix viewXprojection;
}
struct VertexInput{
	float4 pos : Position1;    // Vertex position
	float2 uv : Texcoord;     // Texture coordinates
};
[maxvertexcount(3)]
void main(
	triangle VertexInput input[3],
	inout TriangleStream< GSOutput > output
)
{
	//Should multiply with viewXprojection at last shader stage because of clipping 
	//https://carmencincotti.com/2022-05-02/homogeneous-coordinates-clip-space-ndc/
	//-----------------------------------------------------------------------------//
	float4 p[3] = { mul(input[0].pos, viewXprojection),mul(input[1].pos, viewXprojection),mul(input[2].pos, viewXprojection) };
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = p[i];
		element.vertices = input[i].pos.xyz;
		element.uv = input[i].uv;
		output.Append(element);
	}
}