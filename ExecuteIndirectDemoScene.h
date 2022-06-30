#pragma once
#include "AbstractScene.h"
#include "Camera.h"
#include <memory>
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Model.h"
#include "Object3d.h"
#include "IndirectObject3d.h"
#include <vector>
#include "Emitter.h"

class ExecuteIndirectDemoScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMMATRIX = DirectX::XMMATRIX;

private:

	static const UINT frame_count_ = 3;				// なにこれ
	static const UINT all_particle_num_ = 1;
	//static const UINT all_particle_num_ = 256;
	static const UINT resource_count_ = all_particle_num_ * frame_count_;
	static const UINT command_size_per_frame_;

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
		XMMATRIX mat;		// ３Ｄ変換行列
	};

	// ExecuteIndirect に使用するコマンドシグネチャと一致させるためのデータ構造
	struct IndirectCommand {

		//D3D12_GPU_VIRTUAL_ADDRESS cbv_;
		D3D12_GPU_VIRTUAL_ADDRESS matrix_cbv_;
		D3D12_GPU_VIRTUAL_ADDRESS material_cbv_;
		D3D12_DRAW_INDEXED_ARGUMENTS draw_arguments_;	// DrawIndexedInstancedの場合は使う型が違う
	};

	std::vector<ConstBufferData> const_buffer_data_;
	std::vector<MatrixConstBufferData> matrix_const_buffer_data_;
	std::vector<Model::MaterialConstBufferData> material_const_buffer_data_;

	// パイプラインオブジェクト
	ComPtr<ID3D12Device> device_;
	ComPtr<ID3D12DescriptorHeap> descriptor_heap_;
	UINT descriptor_heap_size_;
	ComPtr<ID3D12CommandSignature> command_signature_;
	ComPtr<ID3D12RootSignature> root_signature_;
	UINT frame_index_;

	// アセットオブジェクト
	ComPtr<ID3D12PipelineState> pipeline_state_;
	ComPtr<ID3D12GraphicsCommandList> command_list_;
	ComPtr<ID3D12Resource> const_buffer_;
	ComPtr<ID3D12Resource> matrix_const_buffer_;
	ComPtr<ID3D12Resource> material_const_buffer_;
	ComPtr<ID3D12Resource> command_buffer_;

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// モデル
	std::unique_ptr<Model> model_;

	// オブジェクト
	std::unique_ptr<Object3d> object_;
	//std::unique_ptr<IndirectObject3d> object_;

	// テストパーティクル
	std::unique_ptr<Emitter> emitter_1_;
	std::unique_ptr<Emitter> emitter_2_;

	ParticleMember p_;
	ParticleMember p_2_;
	XMFLOAT3 range_;
	int generate_num_;

public:

	ExecuteIndirectDemoScene();
	~ExecuteIndirectDemoScene();

private:

	void CreateDescriptorHeap();
	void CreateRootSignature();
	void CreatePipelineState();
	void CreateGraphicsPipeline();
	void CreateConstantBuffer();
	void CreateCommandSignature();
	void CreateCommandBuffer();

	void TransferConstantBafferData();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
