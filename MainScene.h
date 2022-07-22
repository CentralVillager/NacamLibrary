#pragma once
#include "AbstractScene.h"
#include "DirectXBase.h"
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>
#include "Camera.h"
#include "Player.h"

class MainScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	std::unique_ptr<Camera> camera_;

	// プレイヤー
	std::shared_ptr<Player> player_; // uniqueにするとエラーを吐く なぜ [検索]

public:

	MainScene();
	~MainScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
