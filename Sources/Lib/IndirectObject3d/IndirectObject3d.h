#pragma once
#include <memory>
#include <wrl.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <vector>
#include "../../App/Scene/AbstractScene.h"
#include "../Camera/Camera.h"
#include "../Sources/Lib/Model/Model.h"
#include "../Pipeline/PipelineManager.h"

class IndirectObject3d {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	static const UINT frame_count_ = 1;
	static const UINT all_particle_num_ = 100000;
	static const UINT resource_count_ = all_particle_num_ * frame_count_;
	static const UINT command_size_per_frame_;

	struct Vertex {

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

	struct ConstBufferData {

		XMFLOAT3 position_;	// 位置
		float pad1_;
		XMFLOAT3 velocity_;	// 速度
		float pad2_;
		XMFLOAT3 color_;	// 色
		float pad3_;
		//XMMATRIX matrix_;	// 行列
	};

	struct MatrixConstBufferData {
		XMMATRIX mat;	//   64byte
		float pad[48];	// + 48 * 4byte = 256byte
	};

	// ExecuteIndirect に使用するコマンドシグネチャと一致させるためのデータ構造
	struct IndirectCommand {

		D3D12_GPU_VIRTUAL_ADDRESS matrix_cbv_;
		D3D12_GPU_VIRTUAL_ADDRESS material_cbv_;
		D3D12_DRAW_ARGUMENTS draw_arguments_;	// DrawIndexedInstancedの場合は使う型が違う
	};

	std::vector<ConstBufferData> const_buffer_data_;
	std::vector<MatrixConstBufferData> matrix_const_buffer_data_;
	std::vector<Model::MaterialConstBufferData> material_const_buffer_data_;

	// パイプラインオブジェクト
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	UINT descriptor_heap_size_;
	ComPtr<ID3D12CommandSignature> command_signature_;
	static ComPtr<ID3D12RootSignature> root_signature_;
	UINT frame_index_ = 0;

	// アセットオブジェクト
	static ComPtr<ID3D12PipelineState> pipeline_state_;
	static ComPtr<ID3D12GraphicsCommandList> command_list_;
	ComPtr<ID3D12Resource> const_buffer_;
	ComPtr<ID3D12Resource> matrix_const_buffer_;
	ComPtr<ID3D12Resource> material_const_buffer_;
	ComPtr<ID3D12Resource> command_buffer_;
	ComPtr<ID3D12Resource> command_buffer_upload_;
	ComPtr<ID3D12Resource> vertex_buffer_;
	D3D12_VERTEX_BUFFER_VIEW vb_view_;

	XMFLOAT4 color_ = { 1, 1, 1, 1 };
	XMFLOAT3 scale_ = { 1, 1, 1 };
	XMFLOAT3 rotation_ = { 0, 0, 0 };
	XMFLOAT3 position_ = { 0, 0, 0 };
	XMMATRIX mat_world_;
	static Camera *camera_;

private:

	// モデル
	std::unique_ptr<Model> model_;

	bool is_push_ = false;

	static PipelineManager *p_mgr_;

public:

	IndirectObject3d();
	~IndirectObject3d();

private:

	void CreateVertexBuffer();
	void CreateDescriptorHeap();
	void CreateGraphicsPipeline();
	void CreateConstantBuffer();
	void CreateCommandSignature();
	void CreateCommandBuffer();

	void TransferConstantBafferData();

public:

	static void StaticInitialize(const PipelineManager &p_mgr);
	static void SetCamera(Camera *camera) { camera_ = camera; }

	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

	inline void SetPosition(const XMFLOAT3 &position) { position_ = position; }
	inline void SetScale(const float &scale) {

		scale_.x = scale;
		scale_.y = scale;
		scale_.z = scale;
	}

	void DetectPush(bool is_push) { is_push_ = is_push; }
};
