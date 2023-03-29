#pragma once
#include <d3d12.h>
#include <array>
#include <wrl.h>
#include <DirectXMath.h>

enum class Pipeline
{
	Basic,
	AverageBlur,
	GaussianBlur,
	RadialBlur
};

class PostEffect
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;
		XMMATRIX mat;
	};

public:

	PostEffect();
	~PostEffect();

private:

	static ComPtr<ID3D12Device> device_;
	static ComPtr<ID3D12GraphicsCommandList> command_list_;

	// ���_��
	const static UINT vertex_num_ = 4;

	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertex_buffer_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> const_buffer_;

	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vb_view_;

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

	// ��ʃN���A�J���[
	static const float clear_color_[4];

	static XMFLOAT4 color_;

private:

	Pipeline pipeline_;

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

	static float GetAlpha() { return color_.w; }
	static void SetAlpha(float color) { color_.w = color; }

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	void PostDrawScene();
};
