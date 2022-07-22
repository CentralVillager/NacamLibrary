#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET0
{
    float4 color = tex.Sample(smp, input.uv);
    
    float n = 1.5f;

    // n�e�N�Z�����炷���߂�UV�l�����߂�
    float offset_u = n / 1280.0f;
    float offset_v = n / 720.0f;

    // ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(offset_u, 0.0f));
    
    // ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(-offset_u, 0.0f));

    // ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(0.0f, offset_v));

    // ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(0.0f, -offset_v));

    // ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(offset_u, offset_v));
    
    // ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(offset_u, -offset_v));

    // ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(-offset_u, offset_v));

    // ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
    color += tex.Sample(smp, input.uv + float2(-offset_u, -offset_v));

    // ��e�N�Z���Ƌߐ�8�e�N�Z���̕��ςȂ̂�9�ŏ��Z����
    color /= 9.0f;

    return color;
}