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

	// デバイス
	static ComPtr<ID3D12Device> device_;

	// デスクリプタサイズ
	static UINT desc_handle_increment_size_;

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> command_list_;

	// デスクリプタヒープ
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
