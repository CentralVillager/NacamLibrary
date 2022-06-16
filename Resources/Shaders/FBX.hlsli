// �{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer cbuff0 : register(b0)
{
    matrix view_proj;
    matrix world; // ���[���h�s��
    float3 camera_pos; // �J�������W�i���[���h���W�j
};

// �{�[���̃X�L�j���O�s�񂪓���
cbuffer skinning : register(b3)
{
    matrix mat_skinning[MAX_BONES];
};

// ���_�o�b�t�@�̓���
struct VSInput
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    uint4 bone_indices : BONEINDICES; // �{�[���̔ԍ�
    float4 bone_weights : BONEWEIGHTS; // �{�[���̃X�L���E�F�C�g
};

// ���_�V�F�[�_����s�N�Z���V�F�[�_�ւ̂��Ƃ�Ɏg�p����\����
struct VSOutput
{
    float4 sv_pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};