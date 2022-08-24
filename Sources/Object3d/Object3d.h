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
#include "../Model/Model.h"
#include "../Camera/Camera.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	struct MatrixConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private:

	// デバイス
	static ComPtr<ID3D12Device> device;

	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

	// コマンドリスト
	static ComPtr<ID3D12GraphicsCommandList> cmd_list_;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;

	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;

private:

	// 定数バッファ
	ComPtr<ID3D12Resource> material_const_buffer_;
	ComPtr<ID3D12Resource> matrix_const_buffer_;

	// ローカル座標
	XMFLOAT3 position_ = { 0,0,0 };

	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation_ = { 0,0,0 };

	// ローカルスケール
	XMFLOAT3 scale_ = { 1,1,1 };

	// 色
	XMFLOAT4 color_ = { 1,1,1,1 };

	// ローカルワールド変換行列
	XMMATRIX mat_world_;

	// 親オブジェクト
	Object3d *parent_ = nullptr;

	// モデルのポインタ
	Model *model_ptr_ = nullptr;

	// カメラのポインタ
	static Camera *cam_ptr_;

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

private:

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

public:

	/// <summary>
	/// Object3d初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/*-- アクセッサ群 --*/
	inline const XMFLOAT3 &GetPosition() { return position_; }
	inline const XMFLOAT3 &GetRotation() { return rotation_; }
	inline const XMFLOAT3 &GetScale() { return scale_; }
	inline void SetPosition(XMFLOAT3 position) { position_ = position; }
	inline void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	inline void SetScale(XMFLOAT3 scale) { scale_ = scale; };
	inline void SetScale(float scale) { scale_.x = scale_.y = scale_.z = scale; };

	inline const ComPtr<ID3D12Resource> &GetMaterialConstBuffer() { return material_const_buffer_; }
	inline const ComPtr<ID3D12Resource> &GetMatrixConstBuffer() { return matrix_const_buffer_; }

	/// <summary>
	/// モデルデータと3Dオブジェクトの紐づけ
	/// </summary>
	/// <param name="model"></param>
	void SetModel(Model *model) { model_ptr_ = model; }

	static void SetCamera(Camera *camera_) { cam_ptr_ = camera_; }

public:

	const Object3d operator=(const Object3d &rhs);
};
