#include "FBX.hlsli"

// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

// �G���g���[�|�C���g
float4 main(VSOutput input) : SV_Target
{
    // �e�N�X�`���}�b�s���O
    float4 tex_color = tex.Sample(smp, input.uv);
    // Lambert����
    float3 light = normalize(float3(1, -1, 1));
    float diffuse = saturate(dot(-light, input.normal));
    float brightness = diffuse + 0.3f;
    float4 shade_color = float4(brightness, brightness, brightness, 1.0f);
    // �A�e�ƃe�N�X�`���̐F������
    return shade_color * tex_color;
}