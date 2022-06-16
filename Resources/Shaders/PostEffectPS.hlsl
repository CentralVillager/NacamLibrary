#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float4 tex_color = tex.Sample(smp, input.uv);
    
    return float4(tex_color.rgb, 1);
    
    //���x
    //return float4(tex_color.rgb * 2.0f, 1);
    
    // �l�K�|�W���]
    //return float4(1 - tex_color.rgb, 1);
}