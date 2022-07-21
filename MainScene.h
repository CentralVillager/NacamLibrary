#pragma once
#include "AbstractScene.h"
#include <wrl.h>
#include "Camera.h"

class MainScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT3 = DirectX::XMFLOAT3;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// カメラ
	unique_ptr<Camera> camera_;

	// プレイヤー
	//unique_ptr<Player> player_;

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
