struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 collpos :Position2;
};
cbuffer PerFrameBuffer : register (b1) {
	matrix viewXprojection;
}
[maxvertexcount(3)]
void main(
	triangle float4 input[3] : Position1, 
	inout TriangleStream< GSOutput > output
)
{
	float4 p[3] = { mul(input[0], viewXprojection),mul(input[1], viewXprojection),mul(input[2], viewXprojection) };
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = p[i];
		element.collpos = input[i];
		output.Append(element);
	}
}