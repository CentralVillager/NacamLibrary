cbuffer cbuff0 : register(b0)
{
	float4 color;
	matrix mat;
	matrix mat_billboard;
};

struct VSOutput
{
	float4 sv_pos : SV_POSITION;
    float3 normal : NORMAL;
	float uv : TEXCOORD;
};
