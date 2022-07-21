#pragma once
#include "Sprite.h"
#include <array>

enum class Pipeline {

	Basic,
	Blur,
	GaussianBlur
};

class PostEffect : public Sprite {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	PostEffect();

private:

	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> tex_buff_[2];

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> desc_heap_SRV_;

	// �f�v�X�o�b�t�@
	ComPtr<ID3D12Resource> depth_buff_;

	// RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> desc_heap_RTV_;

	// DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> desc_heap_DSV_;

private:

	// ��ʃN���A�J���[
	static const float clear_color_[4];

private:

	// �O���t�B�N�X�p�C�v���C��
	static ComPtr<ID3D12PipelineState> pipeline_state_;
	static ComPtr<ID3D12PipelineState> blur_pipeline_state_;
	static ComPtr<ID3D12PipelineState> gaussian_blur_pipeline_state_;

	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> root_signature_;
	static ComPtr<ID3D12RootSignature> blur_root_signature_;
	static ComPtr<ID3D12RootSignature> gaussian_blur_root_signature_;

	Pipeline pipeline_;

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

	void CreateBlurPipeline();
	void CreateGaussianBlurPipeline();

public:

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �f�o�b�O�`��
	/// </summary>
	void DebugDraw();

public:

	static void SetPipeline(Pipeline p);

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void PostDrawScene();

};
