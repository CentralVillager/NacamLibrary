#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float4 tex_color = tex.Sample(smp, input.uv);
    
    return float4(tex_color.rgb, 1);
    
    //明度
    //return float4(tex_color.rgb * 2.0f, 1);
    
    // ネガポジ反転
    //return float4(1 - tex_color.rgb, 1);
}