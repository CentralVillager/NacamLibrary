cbuffer cbuff0 : register(b0)
{
    matrix mat; // ３Ｄ変換行列
    matrix mat_billboard;
};

struct VSOutput
{
    float4 pos : POSITION;
    float4 scale : TEXCOORD;
    float4 color : COLOR;
    float4 alpha : NORMAL;
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
    float4 color : COLOR;
    float4 alpha : NORMAL;
};
