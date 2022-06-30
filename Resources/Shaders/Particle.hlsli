cbuffer cbuff0 : register(b0)
{
    //matrix mat; // ３Ｄ変換行列
    //matrix mat_billboard;
    
    float3 position : packoffset(c0);
    float3 velocity : packoffset(c1);
    float3 color : packoffset(c2);
    //float4x4 mat : packoffset(c3);
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 pos : POSITION; // システム用頂点座標
    //float4 scale : TEXCOORD; // スケール
    float2 uv : TEXCOORD; // uv値
};

struct GSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};
