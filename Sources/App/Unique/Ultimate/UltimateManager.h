#pragma once
#include <string>
#include <array>
#include <memory>
#include <DirectXMath.h>
#include "../../../Lib/Sprite/NcmSprite.h"
#include "../../Number/Numbers.h"
#include "../Sources/App/Math/Easing/NcmEasing.h"

/// <summary>
/// �A���e�B���b�g�Ɋւ���N���X
/// </summary>
class UltimateManager
{
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT4 = DirectX::XMFLOAT4;

private:

	enum class UltTex
	{
		u, l, t, i, m, a, T, e, stanby
	};

	enum class TexName
	{
		percent
	};

private:

	// �ő�E���g�e�N�X�`����
	static constexpr uint32_t MAX_ULT_TEX_NUM_ = 9;

	static constexpr float MAX_ALPHA_VALUE_ = 1.0f;	// �ő僿�l
	static constexpr float MIN_ALPHA_VALUE_ = 0.3f;	// �ŏ����l

private:

	// �E���g�e�N�X�`���̃n���h���i�[�R���e�i
	static std::array<ncm_thandle, MAX_ULT_TEX_NUM_> ult_tex_hub_;
	static ncm_thandle percent_;
	static ncm_thandle icon_q_;

private:

	uint32_t currect_ult_per_;	// ���E���g��
	uint32_t max_ult_per_;		// �ő�E���g��
	uint32_t value_dist_;		// �C�[�W���O�p�ڕW�l
	bool is_triggering_;		// ����������
	float num_alpha_;			// %�̃��l

	ncm_ehandle ease_;			// �C�[�W���O�n���h��
	ncm_ehandle num_alpha_to_max_;	// %�\���p�C�[�W���O�n���h��
	ncm_ehandle num_alpha_to_min_;	// %�\���p�C�[�W���O�n���h��
	bool to_max_;				// �C�[�W���O�̒l��max�̒l�Ɍ����đJ�ڒ���
	bool to_min_;				// �C�[�W���O�̒l��max�̒l�Ɍ����đJ�ڒ���
	bool is_change_;			// �E���g�̒l���X�V���ꂽ��

	std::unique_ptr<Numbers> nums_;	// ����

	uint32_t ult_tex_draw_intervel_;// �E���g���o�̕`��C���^�[�o��

	bool is_finish_display_;	// �E���g���o���I��������

	float ImGui_num_scale_;
	float ImGui_offset_x_;
	float ImGui_offset_all_y_;
	float ImGui_tex_scale_;
	float ImGui_tex_alpha_;

public:

	UltimateManager();
	~UltimateManager();

public:

	static void LoadResources();

	void Initialize();
	void Update();
	void Draw();
	void DebugDraw();

	/// <summary>
	/// UI�̕`��
	/// </summary>
	void DrawUi();

	/// <summary>
	/// �E���g���o�̕`��
	/// </summary>
	void DrawUltimateStanby();

public:

	/// <summary>
	/// �E���g�������Z����
	/// </summary>
	/// <param name="v"></param>
	void AddUltValue(uint32_t v);

	/// <summary>
	/// �ő�܂ŗ��܂������Ƃ�ʒm����
	/// </summary>
	/// <returns></returns>
	bool NoticeFullCharged();

	/// <summary>
	/// �E���g�𔭓�����
	/// </summary>
	void TriggeringUlt();

private:

	bool DrawULTIMATESequence();

	bool DrawSTANBYSequence();

	bool CheckDoDisplay();
};
