#include "FBX.hlsli"

// スキニング後の頂点・法線が入る
struct SkinOutput
{
    float4 pos;
    float3 normal;
};

SkinOutput ComputeSkin(VSInput input)
{
    // ゼロクリア
    SkinOutput output = (SkinOutput) 0;
    
    uint i_bone; // 計算するボーン番号
    float weight; // ボーンウェイト(重み)
    matrix m; // スキニング行列
    
    // ボーン0
    i_bone = input.bone_indices.x;
    weight = input.bone_weights.x;
    //weight = 1;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    //return output;
    
    // ボーン1
    i_bone = input.bone_indices.y;
    weight = input.bone_weights.y;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    // ボーン2
    i_bone = input.bone_indices.z;
    weight = input.bone_weights.z;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    // ボーン3
    i_bone = input.bone_indices.w;
    weight = input.bone_weights.w;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    return output;
}

// エントリーポイント
VSOutput main(VSInput input)
{
    // スキニング計算
    SkinOutput skinned = ComputeSkin(input);
    // 法線にワールド行列によるスケーリング・回転を適用
    float4 w_normal = normalize(mul(world, float4(skinned.normal, 0)));
    // ピクセルシェーダに渡す値
    VSOutput output;
    // 行列による座標変換
    output.sv_pos = mul(mul(view_proj, world), skinned.pos);
    // ワールド法線を次のステージに渡す
    output.normal = w_normal.xyz;
    // 入力値をそのまま次のステージに渡す
    output.uv = input.uv;
    
    //VSOutput output;
    //output.sv_pos = input.pos;
    //output.normal = input.normal;
    //output.uv = input.uv;
    
    return output;
}