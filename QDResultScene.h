#pragma once
#include "AbstractScene.h"
#include <wrl.h>
#include "DirectXBase.h"
#include "Sprite.h"
#include <memory>

class QDResultScene : public AbstractScene {
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> cmd_list_ = DirectXBase::GetInstance()->GetCommandList().Get();

private:

	// UI
	std::unique_ptr<Sprite> game_over_;

public:

	QDResultScene();
	~QDResultScene();

public:

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	void DebugDraw() override;
};
