#include "BackBoard.h"
#include "ImGuiManager.h"

std::unique_ptr<Model> BackBoard::model_board_;

BackBoard::BackBoard() {

	// モデルがnullなら
	if (!model_board_) {

		// モデルデータを読み込む
		model_board_ = make_unique<Model>();
		model_board_->LoadObjModel("Resources/board/", "board.obj", "board.mtl");
	}

	// 描画データを作成する
	board_ = make_shared<Object3d>();
}

BackBoard::~BackBoard() {
}

void BackBoard::Initialize(XMFLOAT3 pos, bool is_upper) {

	// 描画データを初期化する
	board_->Initialize();

	// モデルをセットする
	board_->SetModel(model_board_.get());

	// 位置を指定する
	board_->SetPosition(pos);

	// スケールを指定する
	board_->SetScale({ 2000.0f, 1.0f, 20.0f });

	// 上側に配置するかどうか決める
	is_upper_ = is_upper;

	// 有効にする
	is_active_ = true;
}

void BackBoard::Finalize() {
}

void BackBoard::Update() {

	// 無効になったら
	if (!is_active_) {

		// 位置をリセットする
		ResetPosition();
	}

	// 有効なら
	if (is_active_) {

		// 動かす
		Move();

		// ボードが奥までいったら
		if (board_->GetPosition().z <= -20.0f) {

			// 無効にする
			is_active_ = false;
		}

		// 更新
		board_->Update();
	}
}

void BackBoard::Draw() {

	// 有効なら
	if (is_active_) {

		// 描画
		board_->Draw();
	}
}

void BackBoard::DebugDraw() {
}

void BackBoard::Move() {

	// 現位置を取得
	XMFLOAT3 pos = board_->GetPosition();

	// z位置を手前にずらす
	pos.z -= 2.0f;

	// 位置をセットする
	board_->SetPosition(pos);
}

BackBoard BackBoard::Activate(XMFLOAT3 pos, bool is_upper) {

	// インスタンスを生成
	BackBoard b;

	// 初期化
	b.Initialize(pos, is_upper);

	// 有効にする
	b.is_active_ = true;

	// 上側に配置するか決める
	b.is_upper_ = is_upper;

	// 返す
	return b;
}

void BackBoard::ResetPosition() {

	// 上側に配置するなら
	if (is_upper_) {

		// この位置
		board_->SetPosition({ 0, 30.0f, 500.0f });

		// それ以外（下）なら
	} else {

		// この位置
		board_->SetPosition({ 0, -30.0f, 500.0f });

	}

	// 有効にする
	is_active_ = true;
}
