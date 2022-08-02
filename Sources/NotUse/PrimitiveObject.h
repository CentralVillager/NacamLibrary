#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include "Sources/Camera/Camera.h"

class PrimitiveObject {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	PrimitiveObject();
	~PrimitiveObject();

public:

	struct Vertex {

		XMFLOAT3 position;
	};

private:

	// �f�o�C�X
	static ComPtr<ID3D12Device> device_;

	// �f�X�N���v�^�T�C�Y
	static UINT desc_handle_increment_size_;

	// �R�}���h���X�g
	static ComPtr<ID3D12GraphicsCommandList> command_list_;

	// �f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

private:

private:

	static Camera *cam_ptr_;

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
};
