#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET0
{
    float4 color = tex.Sample(smp, input.uv);
    
    float n = 1.5f;

    // nテクセルずらすためのUV値を求める
    float offset_u = n / 1280.0f;
    float offset_v = n / 720.0f;

    // 基準テクセルから右のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(offset_u, 0.0f));
    
    // 基準テクセルから左のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(-offset_u, 0.0f));

    // 基準テクセルから下のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(0.0f, offset_v));

    // 基準テクセルから上のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(0.0f, -offset_v));

    // 基準テクセルから右下のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(offset_u, offset_v));
    
    // 基準テクセルから右上のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(offset_u, -offset_v));

    // 基準テクセルから左下のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(-offset_u, offset_v));

    // 基準テクセルから左上のテクセルのカラーをサンプリングする
    color += tex.Sample(smp, input.uv + float2(-offset_u, -offset_v));

    // 基準テクセルと近接8テクセルの平均なので9で除算する
    color /= 9.0f;

    return color;
}