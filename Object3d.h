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

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Object3d {
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	//struct VertexData {
	//	XMFLOAT3 pos;	// xyz座標
	//	XMFLOAT3 normal;// 法線ベクトル
	//	XMFLOAT2 uv;	// uv座標
	//};

	// 行列用定数バッファ
	struct MatrixConstBufferData {
		XMMATRIX mat;	// ３Ｄ変換行列
	};

public: // 静的メンバ関数
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

	/// <summary>
	/// 描画後処理
	/// </summary>
	//static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	static Object3d *Create();

public: // 静的メンバ変数
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

private:// 静的メンバ関数
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	/// <returns></returns>
	static bool InitializeDescriptorHeap();

	/// <summary>
	/// カメラ初期化
	/// </summary>
	//static void InitializeCamera();

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static bool InitializeGraphicsPipeline();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	//static void UpdateViewMatrix();

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3 &GetPosition() { return position_; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(XMFLOAT3 scale) { this->scale_ = scale; };

	/// <summary>
	/// スケールの一括設定
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(float scale) { this->scale_.x = this->scale_.y = this->scale_.z = scale; };

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { position_ = position; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3 &GetRotation() { return rotation_; }

	/// <summary>
	/// スケールの取得
	/// </summary>
	/// <returns></returns>
	const XMFLOAT3 &GetScale() { return scale_; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="position"></param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation_ = rotation; }

	/// <summary>
	/// カメラのポインタのセット
	/// </summary>
	/// <param name="camera"></param>
	static void SetCamera(Camera *camera_) { cam_ptr_ = camera_; }

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> material_const_buffer_;
	ComPtr<ID3D12Resource> matrix_const_buffer_;
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
	Object3d *parent_ = nullptr;
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

	const Object3d operator=(const Object3d &rhs);
};
