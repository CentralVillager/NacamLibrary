#include "FBX.hlsli"

// �X�L�j���O��̒��_�E�@��������
struct SkinOutput
{
    float4 pos;
    float3 normal;
};

SkinOutput ComputeSkin(VSInput input)
{
    // �[���N���A
    SkinOutput output = (SkinOutput) 0;
    
    uint i_bone; // �v�Z����{�[���ԍ�
    float weight; // �{�[���E�F�C�g(�d��)
    matrix m; // �X�L�j���O�s��
    
    // �{�[��0
    i_bone = input.bone_indices.x;
    weight = input.bone_weights.x;
    //weight = 1;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    //return output;
    
    // �{�[��1
    i_bone = input.bone_indices.y;
    weight = input.bone_weights.y;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    // �{�[��2
    i_bone = input.bone_indices.z;
    weight = input.bone_weights.z;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    // �{�[��3
    i_bone = input.bone_indices.w;
    weight = input.bone_weights.w;
    m = mat_skinning[i_bone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);
    
    return output;
}

// �G���g���[�|�C���g
VSOutput main(VSInput input)
{
    // �X�L�j���O�v�Z
    SkinOutput skinned = ComputeSkin(input);
    // �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
    float4 w_normal = normalize(mul(world, float4(skinned.normal, 0)));
    // �s�N�Z���V�F�[�_�ɓn���l
    VSOutput output;
    // �s��ɂ����W�ϊ�
    output.sv_pos = mul(mul(view_proj, world), skinned.pos);
    // ���[���h�@�������̃X�e�[�W�ɓn��
    output.normal = w_normal.xyz;
    // ���͒l�����̂܂܎��̃X�e�[�W�ɓn��
    output.uv = input.uv;
    
    //VSOutput output;
    //output.sv_pos = input.pos;
    //output.normal = input.normal;
    //output.uv = input.uv;
    
    return output;
}