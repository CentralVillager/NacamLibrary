#pragma once
#include "FbxModel.h"
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include "Camera.h"

class FbxObject3d {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:	// �萔
	// �{�[���̍ő吔
	static const int MAX_BONES_ = 32;

public:	// �T�u�N���X
	// �萔�o�b�t�@�\����
	struct ConstBufferDataTransform {
		XMMATRIX mat_view_proj;	// �r���[�v���W�F�N�V�����s��
		XMMATRIX world;			// ���[���h�s��
		XMFLOAT3 camera_pos;	// �J�������W�i���[���h���W�j
	};

	// �萔�o�b�t�@�p�f�[�^�\���́i�X�L�j���O�j
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES_];
	};

protected:
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> const_buffer_transform_;
	// �萔�o�b�t�@�i�X�L���j
	ComPtr<ID3D12Resource> const_buffer_skin_;

	// ���[�J���X�P�[��
	XMFLOAT3 scale_ = { 1, 1, 1 };
	// x, y, z����肎���[�J����]�p
	XMFLOAT3 rotation_ = { 0, 0, 0 };
	// ���[�J�����W
	XMFLOAT3 position_ = { 0, 0, 0 };
	// ���[�J�����[���h�ϊ��s��
	XMMATRIX mat_world_;
	// ���f��
	FbxModel *fbx_model_ptr_ = nullptr;

	// 1�t���[���̎���
	FbxTime frame_time_;
	// �A�j���[�V�����J�n����
	FbxTime start_time_;
	// �A�j���[�V�����I������
	FbxTime end_time_;
	// ���ݎ��ԁi�A�j���[�V�����j
	FbxTime current_time_;
	// �A�j���[�V�����Đ���
	bool is_play_ = false;
	// ���[�v�ݒ�
	bool is_loop_ = false;
	// �Đ����̃A�j���[�V�����ԍ�
	int current_anim_num_;

private:
	// �f�o�C�X
	static ComPtr<ID3D12Device> device_;
	static Camera *camera_;

	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> root_signature_;
	// �p�C�v���C���X�e�[�g
	static ComPtr<ID3D12PipelineState> pipeline_state_;

public:	// �ÓI�����o�֐�
	static void SetDevice(ID3D12Device *device) { device_ = device; }
	static void SetCamera(Camera *camera) { camera_ = camera; }

	/// <summary>
	/// �O���t�B�N�X�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

public:	// �����o�֐�
	// �A�N�Z�b�T
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	void SetPosition(XMFLOAT3 position) { position_ = position; }

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	const XMFLOAT3 &GetPosition() { return position_; }
	const XMFLOAT3 &GetRotation() { return rotation_; }

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmd_list"></param>
	void Draw(ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(FbxModel *model) { fbx_model_ptr_ = model; }

	/// <summary>
	/// �A�j���[�V�����J�n
	/// </summary>
	/// <param name="is_loop">���[�v������Ȃ�true</param>
	/// <param name="anime_enum">�`�悵�����A�j���[�V�����̔ԍ�</param>
	void PlayAnimation(int anime_enum, bool is_loop);

	/// <summary>
	/// �A�j���[�V�����Đ������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsPlaying();

	/// <summary>
	/// �A�j���[�V�������I���������ǂ���
	/// </summary>
	/// <returns>�I�����Ă�����true</returns>
	bool IsAnimationEnd();
};
