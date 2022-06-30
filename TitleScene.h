#pragma once
#include <wrl.h>
#include "AbstractScene.h"
#include "DirectXBase.h"
#include "Sprite.h"
#include "InputManager.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include "Camera.h"
#include "AudioManager.h"
#include "Utility.h"

class TitleScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCmdList().Get();

private:

	// カメラ
	unique_ptr<Camera> camera_;

	// タイトル
	unique_ptr<Sprite> quantum_drive_;

	// BGM
	unique_ptr<AudioManager> bgm_;

public:

	TitleScene();
	~TitleScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
