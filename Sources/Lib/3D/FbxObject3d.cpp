#include "FbxObject3d.h"
#include <d3dcompiler.h>
#include <cassert>
#include "../3D/FbxLoader.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12Device> FbxObject3d::device_ = nullptr;
Camera *FbxObject3d::camera_ = nullptr;
ComPtr<ID3D12RootSignature> FbxObject3d::root_signature_;
ComPtr<ID3D12PipelineState> FbxObject3d::pipeline_state_;

void FbxObject3d::CreateGraphicsPipeline() {
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vs_blob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> ps_blob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> error_blob; // エラーオブジェクト

	assert(device_);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXVS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vs_blob, &error_blob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
					error_blob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/FBXPS.hlsl",    // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&ps_blob, &error_blob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(error_blob->GetBufferSize());

		std::copy_n((char *)error_blob->GetBufferPointer(),
					error_blob->GetBufferSize(),
					errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC input_layout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 影響を受けるボーン番号（4つ）
			"BONEINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // ボーンのスキンウェイト（4つ）
			"BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC g_pipeline{};
	g_pipeline.VS = CD3DX12_SHADER_BYTECODE(vs_blob.Get());
	g_pipeline.PS = CD3DX12_SHADER_BYTECODE(ps_blob.Get());

	// サンプルマスク
	g_pipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	g_pipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	g_pipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blend_desc{};
	blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blend_desc.BlendEnable = true;
	blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	g_pipeline.BlendState.RenderTarget[0] = blend_desc;

	// 深度バッファのフォーマット
	g_pipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	g_pipeline.InputLayout.pInputElementDescs = input_layout;
	g_pipeline.InputLayout.NumElements = _countof(input_layout);

	// 図形の形状設定（三角形）
	g_pipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	g_pipeline.NumRenderTargets = 1;    // 描画対象は1つ
	g_pipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	g_pipeline.SampleDesc.Count = 1;	// 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE desc_range_srv;
	desc_range_srv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER root_params[3];
	// CBV（座標変換行列用）
	root_params[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	root_params[1].InitAsDescriptorTable(1, &desc_range_srv, D3D12_SHADER_VISIBILITY_ALL);
	// CBV（スキニング用）
	root_params[2].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc;
	root_signature_desc.Init_1_0(_countof(root_params), root_params, 1, &sampler_desc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> root_sig_blob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &root_sig_blob, &error_blob);
	// ルートシグネチャの生成
	result = device_->CreateRootSignature(0, root_sig_blob->GetBufferPointer(), root_sig_blob->GetBufferSize(), IID_PPV_ARGS(root_signature_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	g_pipeline.pRootSignature = root_signature_.Get();

	// グラフィックスパイプラインの生成
	result = device_->CreateGraphicsPipelineState(&g_pipeline, IID_PPV_ARGS(pipeline_state_.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

}

void FbxObject3d::Initialize() {
	HRESULT result;

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_transform_)
	);

	// 定数バッファの生成
	result = device_->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&const_buffer_skin_)
	);

	// 定数バッファへデータ転送
	ConstBufferDataSkin *const_map_skin = nullptr;
	result = const_buffer_skin_->Map(0, nullptr, reinterpret_cast<void **>(&const_map_skin));
	for (int i = 0; i < MAX_BONES_; i++) {
		const_map_skin->bones[i] = XMMatrixIdentity();
	}
	const_buffer_skin_->Unmap(0, nullptr);

	// 1フレーム分の時間を60FPSで設定
	frame_time_.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
}

void FbxObject3d::Update() {
	// アニメーション
	if (is_play_) {
		// 1フレーム進める
		current_time_ += frame_time_;

		if (is_loop_ && IsAnimationEnd()) {
			// ループONかつ最後まで再生したら先頭に戻す
			current_time_ = start_time_;
		}
	}

	XMMATRIX mat_scale, mat_rot, mat_trans;

	// スケール、回転、平行移動行列の計算
	mat_scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	mat_rot = XMMatrixIdentity();
	mat_rot *= XMMatrixRotationZ(XMConvertToRadians(rotation_.z));
	mat_rot *= XMMatrixRotationX(XMConvertToRadians(rotation_.x));
	mat_rot *= XMMatrixRotationY(XMConvertToRadians(rotation_.y));
	mat_trans = XMMatrixTranslation(position_.x, position_.y, position_.z);

	// ワールド行列の合成
	mat_world_ = XMMatrixIdentity();	// 変形をリセット
	mat_world_ *= mat_scale;			// ワールド行列にスケーリングを反映
	mat_world_ *= mat_rot;				// ワールド行列に回転を反映
	mat_world_ *= mat_trans;			// ワールド行列に平行移動を反映

	// ビュープロジェクション行列
	const XMMATRIX &mat_view = camera_->GetMatView();
	const XMMATRIX &mat_proj = camera_->GetMatProjection();
	const XMMATRIX &mat_view_proj = mat_view * mat_proj;
	// モデルのメッシュトランスフォーム
	//const XMMATRIX &model_transform = fbx_model_ptr_->GetModelTransform();
	// カメラ座標
	const XMFLOAT3 &camera_pos = camera_->GetEye();

	HRESULT result;
	// 定数バッファへデータ転送
	ConstBufferDataTransform *const_map = nullptr;
	result = const_buffer_transform_->Map(0, nullptr, reinterpret_cast<void **>(&const_map));
	if (SUCCEEDED(result)) {
		/*const_map->mat_view = mat_view;
		const_map->mat_proj = mat_proj;*/
		const_map->mat_view_proj = mat_view_proj;
		//const_map->world = model_transform * mat_world_;
		const_map->world = mat_world_;
		const_map->camera_pos = camera_pos;
		const_buffer_transform_->Unmap(0, nullptr);
	}

	// ボーン配列
	std::vector<FbxModel::Bone> &bones = fbx_model_ptr_->GetBones();

	// 定数バッファへデータ転送
	ConstBufferDataSkin *const_map_skin = nullptr;
	result = const_buffer_skin_->Map(0, nullptr, reinterpret_cast<void **>(&const_map_skin));
	for (int i = 0; i < bones.size(); i++) {
		// 今の姿勢行列
		XMMATRIX mat_current_pose;
		// 今の姿勢行列を取得
		FbxAMatrix fbx_current_pose = bones[i].fbx_cluster->GetLink()->EvaluateGlobalTransform(current_time_);
		// XMMATRIXに変換
		FbxLoader::ConvertMatrixFromFbx(&mat_current_pose, fbx_current_pose);
		// 合成してスキニング行列に
		const_map_skin->bones[i] = bones[i].inverse_initial_pose * mat_current_pose;
	}
	const_buffer_skin_->Unmap(0, nullptr);
}

void FbxObject3d::Draw(ID3D12GraphicsCommandList *cmd_list) {
	// モデルの割り当てが無ければ描画しない
	if (fbx_model_ptr_ == nullptr) {
		return;
	}

	// パイプラインステートの設定
	cmd_list->SetPipelineState(pipeline_state_.Get());
	// ルートシグネチャの設定
	cmd_list->SetGraphicsRootSignature(root_signature_.Get());
	// プリミティブ形状を設定
	cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 定数バッファビューをセット
	cmd_list->SetGraphicsRootConstantBufferView(0, const_buffer_transform_->GetGPUVirtualAddress());
	// 定数バッファビューをセット
	cmd_list->SetGraphicsRootConstantBufferView(2, const_buffer_skin_->GetGPUVirtualAddress());

	// モデル描画
	fbx_model_ptr_->Draw(cmd_list);
}

void FbxObject3d::PlayAnimation(int anime_enum, bool is_loop) {
	// 別のアニメーションを再生する場合
	if (anime_enum != current_anim_num_) {
		// 再生を停止して設定しなおす
		is_play_ = false;
	}

	// 再生中ならなにもしない
	if (is_play_) {
		return;
	}

	FbxScene *fbx_scene = fbx_model_ptr_->GetFbxScene();

	// アニメーション情報取得
	FbxAnimStack *anim_stack = fbx_scene->GetSrcObject<FbxAnimStack>(anime_enum);
	// アニメーションの名前取得
	const char *anim_stack_name = anim_stack->GetName();
	// アニメーションをセット
	fbx_scene->SetCurrentAnimationStack(anim_stack);
	// アニメーションの時間情報
	FbxTakeInfo *take_info = fbx_scene->GetTakeInfo(anim_stack_name);

	// 開始時間取得
	start_time_ = take_info->mLocalTimeSpan.GetStart();
	// 終了時間取得
	end_time_ = take_info->mLocalTimeSpan.GetStop();
	// 開始時間に合わせる
	current_time_ = start_time_;
	// 再生中状態にする
	is_play_ = true;
	// ループ設定
	is_loop_ = is_loop;
	// 現在のアニメ番号
	current_anim_num_ = anime_enum;
}

bool FbxObject3d::IsPlaying() {
	if (is_play_) { return true; }

	return false;
}

bool FbxObject3d::IsAnimationEnd() {
	if (current_time_ > end_time_) {
		return true;
	}

	return false;
}

// 異常終了する場合は、nullptrならreturnするように書き換える
// 何がnullptrになるのか現状分からない！
// ので一回異常終了を経験する必要がある
