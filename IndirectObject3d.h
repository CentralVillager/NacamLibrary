#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Model.h"
#include "Camera.h"

class IndirectObject3d {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT4X4 = DirectX::XMFLOAT4X4;

public:

	// 行列用定数バッファ
	struct MatrixConstBufferData {
		XMMATRIX mat;
	};

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

public: // 静的メンバ変数

	// デバイス
	static ComPtr<ID3D12Device> device_;
	// デスクリプタサイズ
	static UINT descriptor_handle_increment_size_;
	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature_;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate_;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> desc_heap_;

	static const UINT frame_count_ = 3;				// なにこれ
	static const UINT all_particle_num_ = 256;
	static const UINT resource_count_ = all_particle_num_ * frame_count_;
	static const UINT command_size_per_frame_;

private:

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

private:

	// コマンドシグネチャ
	ComPtr<ID3D12CommandSignature> command_signature_;

	// コマンドバッファ
	ComPtr<ID3D12Resource> command_buffer_;

	UINT frame_index_;

public:

	void CreateCommandSignature();
	void CreateCommandBuffer();

	void Initialize();
	void Update();
	void Draw();

	const XMFLOAT3 &GetPosition() { return position_; }
	const XMFLOAT3 &GetRotation() { return rotation_; }
	const XMFLOAT3 &GetScale() { return scale_; }
	void SetPosition(XMFLOAT3 position) { position_ = position; }
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	void SetScale(XMFLOAT3 scale) { scale_ = scale; };
	void SetScale(float scale) { scale_.x = scale_.y = scale_.z = scale; };
	static void SetCamera(Camera *camera_) { cam_ptr_ = camera_; }

private:

	struct ConstBufferData {

		XMFLOAT3 position_;	// 位置
		float pad1_;
		XMFLOAT3 velocity_;	// 速度
		float pad2_;
		XMFLOAT3 color_;	// 色
		float pad3_;
		XMFLOAT4X4 matrix_;	// 行列
	};

	// ExecuteIndirect に使用するコマンドシグネチャと一致させるためのデータ構造
	struct IndirectCommand {

		D3D12_GPU_VIRTUAL_ADDRESS cbv_;
		D3D12_DRAW_ARGUMENTS draw_arguments_;
	};

private:

	// 定数バッファ
	static ComPtr<ID3D12Resource> material_const_buffer_;
	static ComPtr<ID3D12Resource> matrix_const_buffer_;
	static ComPtr<ID3D12Resource> const_buffer_;
	// 色
	XMFLOAT4 color_ = { 1,1,1,1 };
	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };
	// ローカルワールド変換行列
	XMMATRIX mat_world_;
	// 親オブジェクト
	IndirectObject3d *parent_ = nullptr;
	// モデルのポインタ
	Model *model_ptr_ = nullptr;
	// カメラのポインタ
	static Camera *cam_ptr_;

public:

	const ComPtr<ID3D12Resource> &GetMaterialConstBuffer() { return material_const_buffer_; }
	const ComPtr<ID3D12Resource> &GetMatrixConstBuffer() { return matrix_const_buffer_; }

	/// <summary>
	/// モデルデータと3Dオブジェクトの紐づけ
	/// </summary>
	/// <param name="model"></param>
	void SetModel(Model *model) { model_ptr_ = model; }

	const IndirectObject3d operator=(const IndirectObject3d &rhs);
};
