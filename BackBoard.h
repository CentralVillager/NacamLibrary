#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Model.h"
#include "Object3d.h"

class BackBoard {
	using XMFLOAT3 = DirectX::XMFLOAT3;

private:

	// ボードモデル
	static std::unique_ptr<Model> model_board_;

	// ボードオブジェクト
	std::shared_ptr<Object3d> board_ = nullptr;

	// 有効か
	bool is_active_ = false;

	// 上に配置するかどうか
	bool is_upper_ = false;

public:

	BackBoard();
	~BackBoard();

public:

	void Initialize(XMFLOAT3 pos, bool is_upper);
	void Finalize();
	void Update();
	void Draw();
	void DebugDraw();

public:

	// 有効にする
	static BackBoard Activate(XMFLOAT3 pos, bool is_upper);

	// 移動させる
	void Move();

	// 位置をリセットする
	void ResetPosition();
};
