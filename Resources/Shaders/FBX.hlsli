// ボーンの最大数
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
    matrix view_proj;
    matrix world; // ワールド行列
    float3 camera_pos; // カメラ座標（ワールド座標）
};

// ボーンのスキニング行列が入る
cbuffer skinning : register(b3)
{
    matrix mat_skinning[MAX_BONES];
};

// 頂点バッファの入力
struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 bone_indices : BONEINDICES; // ボーンの番号
    float4 bone_weights : BONEWEIGHTS; // ボーンのスキンウェイト
};

// 頂点シェーダからピクセルシェーダへのやりとりに使用する構造体
struct VSOutput
{
    float4 sv_pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};
