#include "Basic.hlsli"

//Texture2D<float4> tex : register(t0);	// 0番スロットに設定されたテクスチャ
//SamplerState smp : register(s0);		// 0番スロットに設定されたサンプラー
//
//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1, -1, 1));						// 右下奥 向きのライト
//	float diffuse = saturate(dot(-light, input.normal));			// diffuseを[0, 1]の範囲にclampする
//	float brightness = diffuse + 0.3f;								// アンビエント項を0.3として計算
//	float4 texcolor = float4(tex.Sample(smp, input.uv));
//	return float4(texcolor.rgb * brightness, texcolor.a) * color;	// 輝度をRGBに代入して出力
//}

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSOutput
{
    float4 target0 : SV_Target0;
    float4 target1 : SV_Target1;
};

PSOutput main(VSOutput input)
{
   PSOutput output;
   float3 light = normalize(float3(1, -1, 1)); // 右下奥向きのライト
   float light_diffuse = saturate(dot(-light, input.normal));
   float brightness = light_diffuse + 0.3f;
   float4 shade_color = float4(brightness, brightness, brightness, 1.0f);
   //shade_color = m_ambient; // アンビエント値
   //shade_color += m_diffuse * light_diffuse; // ディフューズ項
   float4 texcolor = tex.Sample(smp, input.uv);
    
   output.target0 = shade_color * texcolor;
   output.target1 = float4(1 - (shade_color * texcolor).rgb, 1);
    
   return output;
   //return float4(texcolor.rgb * shade_color, texcolor.a * m_alpha);
}

// float4 main() : SV_Target0
// {
//     return float4(1.0f, 1.0f, 1.0f, 1.0f);
// }

//float4 main(VSOutput input) : SV_Target
//{
//    float4 color_tex_0 = tex0.Sample(smp, input.uv);
//    float4 color_tex_1 = tex1.Sample(smp, input.uv);
    
//    float4 color = color_tex_0;
//    if (fmod(input.uv.y, 0.1f) < 0.05f)
//    {
//        color = color_tex_1;
//    }
    
//    return float4(color.rgb, 1);
//}