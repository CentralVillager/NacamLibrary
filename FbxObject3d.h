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

public:	// 定数
	// ボーンの最大数
	static const int MAX_BONES_ = 32;

public:	// サブクラス
	// 定数バッファ構造体
	struct ConstBufferDataTransform {
		XMMATRIX mat_view_proj;	// ビュープロジェクション行列
		XMMATRIX world;			// ワールド行列
		XMFLOAT3 camera_pos;	// カメラ座標（ワールド座標）
	};

	// 定数バッファ用データ構造体（スキニング）
	struct ConstBufferDataSkin {
		XMMATRIX bones[MAX_BONES_];
	};

protected:
	// 定数バッファ
	ComPtr<ID3D12Resource> const_buffer_transform_;
	// 定数バッファ（スキン）
	ComPtr<ID3D12Resource> const_buffer_skin_;

	// ローカルスケール
	XMFLOAT3 scale_ = { 1, 1, 1 };
	// x, y, z軸回りｎローカル回転角
	XMFLOAT3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	XMFLOAT3 position_ = { 0, 0, 0 };
	// ローカルワールド変換行列
	XMMATRIX mat_world_;
	// モデル
	FbxModel *fbx_model_ptr_ = nullptr;

	// 1フレームの時間
	FbxTime frame_time_;
	// アニメーション開始時間
	FbxTime start_time_;
	// アニメーション終了時間
	FbxTime end_time_;
	// 現在時間（アニメーション）
	FbxTime current_time_;
	// アニメーション再生中
	bool is_play_ = false;
	// ループ設定
	bool is_loop_ = false;
	// 再生中のアニメーション番号
	int current_anim_num_;

private:
	// デバイス
	static ComPtr<ID3D12Device> device_;
	static Camera *camera_;

	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> root_signature_;
	// パイプラインステート
	static ComPtr<ID3D12PipelineState> pipeline_state_;

public:	// 静的メンバ関数
	static void SetDevice(ID3D12Device *device) { device_ = device; }
	static void SetCamera(Camera *camera) { camera_ = camera; }

	/// <summary>
	/// グラフィクスパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

public:	// メンバ関数
	// アクセッサ
	void SetScale(XMFLOAT3 scale) { scale_ = scale; }
	void SetRotation(XMFLOAT3 rotation) { rotation_ = rotation; }
	void SetPosition(XMFLOAT3 position) { position_ = position; }

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3 &GetPosition() { return position_; }
	const XMFLOAT3 &GetRotation() { return rotation_; }

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmd_list"></param>
	void Draw(ID3D12GraphicsCommandList *cmd_list);

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel *model) { fbx_model_ptr_ = model; }

	/// <summary>
	/// アニメーション開始
	/// </summary>
	/// <param name="is_loop">ループさせるならtrue</param>
	/// <param name="anime_enum">描画したいアニメーションの番号</param>
	void PlayAnimation(int anime_enum, bool is_loop);

	/// <summary>
	/// アニメーション再生中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsPlaying();

	/// <summary>
	/// アニメーションが終了したかどうか
	/// </summary>
	/// <returns>終了していたらtrue</returns>
	bool IsAnimationEnd();
};
